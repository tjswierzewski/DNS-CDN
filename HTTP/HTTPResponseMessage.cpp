#include "HTTPResponseMessage.h"

/**
 * Create Response message from string
 */
HTTPResponseMessage::HTTPResponseMessage(std::string buffer) : HTTPMessage(buffer)
{
    int delim;
    delim = buffer.find_first_of(" ");
    this->version = std::stof(buffer.substr(5, delim - 5));
    buffer.erase(0, delim + 1);
    delim = buffer.find_first_of(" ");
    this->status = std::stoi(buffer.substr(0, delim));
    buffer.erase(0, delim + 1);
    delim = buffer.find_first_of("\r\n");
    this->statusMessage = buffer.substr(0, delim);
}
/**
 * Create Response message from arguments
 */
HTTPResponseMessage::HTTPResponseMessage(float version, int status, std::string statusMessage, HTTPMessage::headerMap headers, std::string data) : HTTPMessage(version, headers, data)
{
    this->status = status;
    this->statusMessage = statusMessage;
}
/**
 * Get response status
 */
int HTTPResponseMessage::getStatus()
{
    return this->status;
}
/**
 *  Get response status message
 */
std::string HTTPResponseMessage::getStatusMessage()
{
    return this->statusMessage;
}
/**
 * Return string of request formatted to be sent via socket
 */
std::string HTTPResponseMessage::format()
{
    std::string output;
    output += "HTTP/";
    output += this->printVersion();
    output += " ";
    output += std::to_string(this->getStatus());
    output += " ";
    output += this->getStatusMessage();
    output += "\r\n";
    output += HTTPMessage::format();
    return output;
}