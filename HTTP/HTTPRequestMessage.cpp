#include "HTTPRequestMessage.h"

/**
 * Create HTTP request with strings as arguments
 */
HTTPRequestMessage::HTTPRequestMessage(float version, std::string method, std::string path, HTTPMessage::headerMap headers, std::string data) : HTTPMessage(headers, data)
{
    this->version = version;
    this->method = method;
    this->path = path;
}

/**
 * Get request HTTP method
 */
std::string HTTPRequestMessage::getMethod()
{
    return this->method;
}
/**
 * Get request path
 */
std::string HTTPRequestMessage::getPath()
{
    return this->path;
}
/**
 * Set request path
 */
void HTTPRequestMessage::setPath(std::string path)
{
    this->path = path;
}
/**
 * Return string of request formatted to be sent via socket
 */
std::string HTTPRequestMessage::format()
{
    std::string output;
    output += this->method;
    output += " ";
    output += this->path;
    output += " HTTP/";
    output += printVersion();
    output += "\r\n";
    for (auto &[key, value] : this->headers)
    {
        output += key;
        output += ": ";
        output += value;
        output += "\r\n";
    }
    output += "\r\n";

    output += data;
    output += "\0";

    return output;
}
