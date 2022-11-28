#ifndef HTTP_RESPONSE_MESSAGE_H
#define HTTP_RESPONSE_MESSAGE_H

#include <string>
#include "HTTPMessage.h"

class HTTPResponseMessage : public HTTPMessage
{
protected:
    int status;
    std::string statusMessage;

public:
    /**
     * Create Response message from string
     */
    HTTPResponseMessage(std::string buffer);
    /**
     * Create Response message from arguments
     */
    HTTPResponseMessage(float version, int status, std::string statusMessage, HTTPMessage::headerMap headers, std::string data = "");
    /**
     * Get response status
     */
    int getStatus();
    /**
     *  Get response status message
     */
    std::string getStatusMessage();
    /**
     * Return string of request formatted to be sent via socket
     */
    std::string format() override;
};

#endif