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
