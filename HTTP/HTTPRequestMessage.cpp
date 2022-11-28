#include "HTTPRequestMessage.h"

/**
 * Create HTTP request with strings as arguments
 */
HTTPRequestMessage::HTTPRequestMessage(float version, std::string method, std::string path, HTTPMessage::headerMap headers, std::string data) : HTTPMessage(version, headers, data)
{
    this->version = version;
    this->method = method;
    this->path = path;
}
/**
 * Create HTTP request from buffer
 */
HTTPRequestMessage::HTTPRequestMessage(std::string buffer) : HTTPMessage(buffer)
{
    int delim;
    delim = buffer.find_first_of(" ");
    this->method = buffer.substr(0, delim);
    buffer.erase(0, delim + 1);
    delim = buffer.find_first_of(" ");
    this->path = buffer.substr(0, delim);
    buffer.erase(0, delim + 1);
    delim = buffer.find_first_of("\r\n");
    this->version = stof(buffer.substr(5, delim - 5));
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
    output += this->printVersion();
    output += "\r\n";
    output += HTTPMessage::format();
    return output;
}
