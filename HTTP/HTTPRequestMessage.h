#ifndef HTTP_REQUEST_MESSAGE_H
#define HTTP_REQUEST_MESSAGE_H

#include "HTTPMessage.h"
#include <string>

class HTTPRequestMessage : public HTTPMessage
{
protected:
    std::string method;
    std::string path;

public:
    /**
     * Create HTTP request with strings as arguments
     */
    HTTPRequestMessage(float version, std::string method, std::string path, HTTPMessage::headerMap headers, std::string data = "");
    /**
     * Get request HTTP method
     */
    std::string getMethod();
    /**
     * Get request path
     */
    std::string getPath();
    /**
     * Set request path
     */
    void setPath(std::string path);
    /**
     * Return string of request formatted to be sent via socket
     */
    std::string format();
};

#endif