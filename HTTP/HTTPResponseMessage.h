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
     * Get response status
     */
    int getStatus();

    /**
     *  Get response status message
     */
    std::string getStatusMessage();
};

#endif