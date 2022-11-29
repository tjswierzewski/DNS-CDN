#include "HTTPSession.h"
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

#define BUFFER_SIZE 1 << 16

/**
 * Create HTTP Session object
 */
HTTPSession::HTTPSession(int fd)
{
    this->fd = this->acceptConnection(fd);
    if (this->fd < 0)
    {
        throw std::invalid_argument("Error in connection");
    }
};
/**
 * Get HTTP Session file descriptor
 */
int HTTPSession::getFD()
{
    return this->fd;
}
/**
 * Accept incoming socket connection
 */
int HTTPSession::acceptConnection(int fd)
{
    struct sockaddr_in clientAddress;
    int conn_sock;
    int clientAddrLength = sizeof(clientAddress);

    conn_sock = accept4(fd, (struct sockaddr *)&clientAddress, (socklen_t *)&clientAddrLength, SOCK_NONBLOCK);
    if (conn_sock == -1)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    return conn_sock;
}

/**
 * Send Get request to Host
 */
HTTPResponseMessage *HTTPSession::get(std::string path)
{
    HTTPMessage::headerMap headers;
    HTTPRequestMessage request(1.1, HTTPMethod::Get, path, headers);
    this->write(&request);
    return (HTTPResponseMessage *)this->read(HTTP_RESPONSE);
}

/**
 * Write message over socket
 */
void HTTPSession::write(HTTPMessage *message)
{
    int rc;
    if (!this->cookies.empty())
    {
        message->setHeader("Cookie", this->sendCookies());
    }

    std::string output = message->format();
    rc = ::write(this->fd, output.c_str(), output.length());
    while (rc < output.length())
    {
        rc += ::write(this->fd, (char *)output.c_str() + rc, output.length() - rc);
    }
}

/**
 * Update session state
 */
void HTTPSession::updateSession(HTTPMessage *message)
{
    for (auto &[key, value] : message->getHeaders())
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
void HTTPSession::setCookie(std::string cookie)
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
std::string HTTPSession::sendCookies()
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
HTTPResponseMessage *HTTPSession::post(std::string path, std::string data, std::string type)
{
    HTTPMessage::headerMap headers;
    headers.insert(std::make_pair("Content-Length", std::to_string(data.length())));
    headers.insert(std::make_pair("Content-Type", type));
    HTTPRequestMessage request(1.1, HTTPMethod::Post, path, headers, data);
    this->write(&request);
    return (HTTPResponseMessage *)this->read(HTTP_RESPONSE);
}
/**
 * Read message from socket
 */
HTTPMessage *HTTPSession::read(int type)
{
    HTTPMessage *message;
    char buffer[BUFFER_SIZE];
    std::ostringstream input;
    int rc = 0;
    while (input.str().find("\r\n\r\n") == std::string::npos)
    {
        rc += ::read(this->fd, buffer + rc, 1);
        if (rc == 0 && errno == 0)
        {
            close(this->fd);
            return NULL;
        }
        input << buffer[rc - 1];
    }
    if (type == HTTP_REQUEST)
    {
        message = new HTTPRequestMessage(input.str());
    }
    else
    {
        message = new HTTPResponseMessage(input.str());
    }

    char *content = buffer + rc;
    if (message->getHeaders().count("Content-Length"))
    {
        rc = 0;
        while (rc < std::stoi(message->getHeaders().find("Content-Length")->second))
        {
            rc += ::read(this->fd, content + rc, std::stoi(message->getHeaders().find("Content-Length")->second) - rc);
        }
    }
    std::string data(content);
    message->setData(data);
    this->updateSession(message);
    return message;
}
