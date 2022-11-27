#include "HTTPSSession.h"
#include "HTTPMessage.h"
#include "HTTPResponseMessage.h"
#include "HTTPRequestMessage.h"
#include "HTTPMethod.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>

#define BUFFER_SIZE 100000

/**
 * Create HTTP Session object
 */
HTTPSSession::HTTPSSession(const char *host, const char *port)
{
    this->host = host;
    this->port = port;
    this->socket = connectToHost();
    if (this->socket < 0)
    {
        throw std::invalid_argument("Error in port creation");
    }
};
/**
 * Delete HTTP Session object
 */
HTTPSSession::~HTTPSSession()
{
    close(this->socket);
}

/**
 * Create socket connection with Host
 */
int HTTPSSession::connectToHost()
{
    struct addrinfo hints, *addr;
    int sock, err;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    if ((err = getaddrinfo(this->host, this->port, &hints, &addr)) != 0)
    {
        std::cout << "Error" << err << ": " << gai_strerror(err) << std::endl;
        return -1;
    }

    sock = ::socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (sock < 0)
    {
        perror("socket");
        return -1;
    }

    if (connect(sock, addr->ai_addr, addr->ai_addrlen) != 0)
    {
        perror("connect");
        return -1;
    }
    return sock;
}

/**
 * Send Get request to Host
 */
HTTPResponseMessage HTTPSSession::get(std::string path)
{
    HTTPMessage::headerMap headers;
    HTTPRequestMessage request(1.1, HTTPMethod::Get, path, headers);
    return this->send(request);
}

/**
 * Send Post request to Host
 */
HTTPResponseMessage HTTPSSession::send(HTTPRequestMessage request)
{
    int rc;
    char buffer[BUFFER_SIZE];
    std::ostringstream input;
    memset(buffer, 0, sizeof(buffer));
    request.setHeader("HOST", this->host);
    request.setHeader("USER-AGENT", "Webcrawler/TS");
    if (!this->cookies.empty())
    {
        request.setHeader("Cookie", this->sendCookies());
    }

    std::string output = request.format();
    rc = SSL_write(ssl, output.c_str(), output.length());
    while (rc < output.length())
    {
        rc += SSL_write(ssl, (char *)output.c_str() + rc, output.length() - rc);
    }
    rc = 0;
    while (input.str().find("\r\n\r\n") == std::string::npos)
    {
        rc += SSL_read(ssl, buffer + rc, 1);
        input << buffer[rc - 1];
    }
    HTTPResponseMessage response(buffer);
    char *content = buffer + rc;
    if (response.getHeaders().count("Content-Length"))
    {
        rc = 0;
        while (rc < std::stoi(response.getHeaders().find("Content-Length")->second))
        {
            rc += SSL_read(ssl, content + rc, std::stoi(response.getHeaders().find("Content-Length")->second) - rc);
        }
    }
    std::string data(content);
    response.setData(data);
    this->updateSession(response);
    if (response.getStatus() == 302 || response.getStatus() == 301)
    {
        request.setPath(response.getHeaders().find("Location")->second);
        response = this->send(request);
    }

    return response;
}

/**
 * Update session state
 */
void HTTPSSession::updateSession(HTTPResponseMessage response)
{
    for (auto &[key, value] : response.getHeaders())
    {
        if (key == "Set-Cookie")
        {
            this->setCookie(value);
        }
    }
}

/**
 * Set value of cookie
 */
void HTTPSSession::setCookie(std::string cookie)
{
    std::string key, value;
    int stringPointer;
    stringPointer = cookie.find("=");
    key = cookie.substr(0, stringPointer);
    cookie.erase(0, stringPointer + 1);
    stringPointer = cookie.find(";");
    value = cookie.substr(0, stringPointer);
    this->cookies[key] = value;
}

/**
 * Format cookies in string to be sent in headers
 */
std::string HTTPSSession::sendCookies()
{
    std::ostringstream buffer;
    for (auto &[key, value] : this->cookies)
    {
        buffer << key << "=" << value << "; ";
    }
    std::string output = buffer.str();
    output.erase(output.rfind(";"));
    return output;
}

/**
 * Send Post request to Host
 */
HTTPResponseMessage HTTPSSession::post(std::string path, std::string data, std::string type)
{
    HTTPMessage::headerMap headers;
    headers.insert(std::make_pair("Content-Length", std::to_string(data.length())));
    headers.insert(std::make_pair("Content-Type", type));
    HTTPRequestMessage request(1.1, HTTPMethod::Post, path, headers, data);
    return this->send(request);
}