#include "HTTPMessage.h"
#include <string>
#include <iomanip>
#include <sstream>

/**
 * Create HTTP message out of buffer
 */
HTTPMessage::HTTPMessage(std::string buffer)
{
    int delim = buffer.find_first_of("\r\n");
    buffer.erase(0, delim + 2);
    parseHeaders(&buffer);
    this->data = buffer;
}

/**
 * Create HTTP message out of components
 */
HTTPMessage::HTTPMessage(headerMap headers, std::string data)
{
    this->headers = headers;
    this->data = data;
}

/**
 * Get HTTP version
 */
float HTTPMessage::getVersion()
{
    return this->version;
}

/**
 * Get HTTP headers
 */
HTTPMessage::headerMap HTTPMessage::getHeaders()
{
    return this->headers;
}

/**
 * Set a single header value
 */
void HTTPMessage::setHeader(std::string key, std::string value)
{
    this->headers.insert(std::pair<std::string, std::string>(key, value));
}

/**
 * Get HTTP message data
 */
std::string HTTPMessage::getData()
{
    return this->data;
}

/**
 * Create headers out of string
 */
void HTTPMessage::parseHeaders(std::string *buffer)
{
    std::string header, value;
    int delim, end;
    while (buffer->find_first_of("\r\n\r\n") != 0)
    {
        delim = buffer->find_first_of(':');
        header = buffer->substr(0, delim);
        buffer->erase(0, delim + 2);
        delim = buffer->find_first_of("\r\n");
        value = buffer->substr(0, delim);
        buffer->erase(0, delim + 2);
        this->headers.insert(std::pair<std::string, std::string>(header, value));
    }
    buffer->erase(0, 2);
}
/**
 * Get String of version
 */
std::string HTTPMessage::printVersion()
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << this->version;
    return ss.str();
}

/**
 * Set message data
 */
void HTTPMessage::setData(std::string data)
{
    this->data = data;
}