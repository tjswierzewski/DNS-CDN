#ifndef HTTPSession_H
#define HTTPSession_H

#include <map>
#include <string>
#include "HTTPResponseMessage.h"
#include "HTTPRequestMessage.h"

class HTTPSession
{
public:
    typedef std::map<std::string, std::string> cookiesMap;

private:
    cookiesMap cookies;
    int fd;

    /**
     * Create socket connection with Host
     */
    int connectToHost();
    /**
     * Accept incoming socket connection
     */
    int acceptConnection(int fd);
    /**
     * Send HTTP message
     */
    HTTPResponseMessage send(HTTPRequestMessage);
    /**
     * Update session state
     */
    void updateSession(HTTPResponseMessage response);
    /**
     * Set value of cookie
     */
    void setCookie(std::string value);
    /**
     * Format cookies in string to be sent in headers
     */
    std::string sendCookies();

public:
    /**
     * Create HTTPS Session object
     */
    HTTPSession(int fd);
    /**
     * Get HTTP Session file descriptor
     */
    int getFD();
    /**
     * Send Get request to Host
     */
    HTTPResponseMessage get(std::string path);
    /**
     * Send Post request to Host
     */
    HTTPResponseMessage post(std::string path, std::string data, std::string type);
    /**
     * Read message from socket
     */
    int read();
};
#endif