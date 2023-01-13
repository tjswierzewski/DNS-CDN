#include "HTTPSession.h"
#include "HTTPMessage.h"
#include "HTTPResponseMessage.h"
#include "HTTPRequestMessage.h"
#include "HTTPMethod.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>

#define BUFFER_SIZE 1 << 20

/**
 * Create HTTP Session from listen socket
 */
HTTPSession::HTTPSession(int fd, int epoll)
{
    this->epoll = epoll;
    this->fd = this->acceptConnection(fd);
    if (this->fd < 0)
    {
        throw std::invalid_argument("Error in connection");
    }
};
/**
 * Create HTTPS Session connected to provided host
 */
HTTPSession::HTTPSession(const char *host, const char *port, int epoll)
{
    this->epoll = epoll;
    this->host = host;
    this->port = port;
    this->fd = connectToHost();
    if (this->fd < 0)
    {
        throw std::invalid_argument("Error in port creation");
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
 * Create socket connection with Host
 */
int HTTPSession::connectToHost()
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
    request.setHeader("HOST", this->host);
    request.setHeader("USER-AGENT", "Webcrawler/TS");
    this->write(&request);
    return (HTTPResponseMessage *)this->read(HTTP_RESPONSE);
}

/**
 * Write message over socket
 */
void HTTPSession::write(HTTPMessage *message)
{
    int rc = 0;
    if (!this->cookies.empty())
    {
        message->setHeader("Cookie", this->sendCookies());
    }
    if (message->getDataLength() != 0)
    {
        message->removeHeader("Transfer-Encoding");
        for (auto &[key, value] : message->getHeaders())
        {
            if (key == "Content-Length")
            {
                message->removeHeader("Content-Length");
            }
        }
        message->setHeader("Content-Length", std::to_string(message->getDataLength()));
    }
    std::string output = message->format();
    std::cout << output.length() << std::endl;
    for (auto &[key, value] : message->getHeaders())
    {
        if (key == "Content-Length")
        {
            std::cout << "c-length" << value << std::endl;
        }
    }

    int timeout = 0;
    char *out_buffer = (char *)output.c_str();
    int out_count = output.length();
    while (rc < output.length())
    {
        int temp = ::write(this->fd, out_buffer + rc, out_count - rc);
        std::cout << "write fd = " << this->fd << std::endl;
        std::cout << "write temp = " << temp << std::endl;
        std::cout << "write rc = " << rc << std::endl;
        std::cout << "write errno = " << errno << std::endl;
        if (temp < 0)
        {
            perror("write");
            if (errno == 11)
            {
                if (timeout < 10)
                {
                    std::cout << "write timeout = " << timeout << std::endl;
                    timeout++;
                }
                else
                {
                    return;
                }
            }

            continue;
        }
        else
        {
            rc += temp;
        }
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
    std::cout << "read" << std::endl;
    while (input.str().find("\r\n\r\n") == std::string::npos)
    {
        rc += ::read(this->fd, buffer + rc, 1);
        if (rc == 0)
        {
            std::cout << "read rc = " << rc << std::endl;
            std::cout << "read errno = " << errno << std::endl;
            if (errno == 0)
            {
                return NULL;
            }
            perror("read");
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                return NULL;
            }
            else if (errno == 2 | errno == 9)
            {
                std::cout << "WELCOME TO THE DANGER ZONE" << std::endl;
                return NULL;
            }
            else
            {
                return NULL;
            }
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
    input.flush();
    rc = 0;
    if (message->getHeaders().count("Content-Length"))
    {
        while (rc < std::stoi(message->getHeaders().find("Content-Length")->second))
        {
            rc += ::read(this->fd, content + rc, std::stoi(message->getHeaders().find("Content-Length")->second) - rc);
        }
    }
    else if (message->getHeaders().count("Transfer-Encoding") &&
             message->getHeaders().find("Transfer-Encoding")->second.compare("chunked") == 0)
    {
        char size[10];
        std::ostringstream sizeString;
        while (sizeString.str().find("\r\n") == std::string::npos)
        {
            rc += ::read(this->fd, size + rc, 1);
            if (rc == 0 && errno == 0)
            {
                return NULL;
            }
            sizeString << size[rc - 1];
            rc = 0;
        }
        std::cout << sizeString.str() << std::endl;
        int dataSize = std::stoul(sizeString.str().substr(0, sizeString.str().size() - 2), nullptr, 16);
        int data_rc = 0;
        while (data_rc < dataSize)
        {
            data_rc += ::read(this->fd, content + data_rc, dataSize - data_rc);
        }
    }

    std::string data(content);
    message->setData(data);
    this->updateSession(message);
    return message;
}
