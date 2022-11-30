#ifndef HTTPSession_H
#define HTTPSession_H

#include <map>
#include <string>
#include "HTTPResponseMessage.h"
#include "HTTPRequestMessage.h"

#define HTTP_REQUEST 0
#define HTTP_RESPONSE 1

class HTTPSession
{
public:
    typedef std::map<std::string, std::string> cookiesMap;

private:
    cookiesMap cookies;
    int fd;
    const char *host, *port;

    /**
     * Create socket connection with Host
     */
    int connectToHost();
    /**
     * Accept incoming socket connection
     */
    int acceptConnection(int fd);

    /**
     * Update session state
     */
    void updateSession(HTTPMessage *message);
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
     * Create HTTP Session from listen socket
     */
    HTTPSession(int fd);
    /**
     * Create HTTPS Session connected to provided host
     */
    HTTPSession(const char *host, const char *port);
    /**
     * Get HTTP Session file descriptor
     */
    int getFD();
    /**
     * Send Get request to Host
     */
    HTTPResponseMessage *get(std::string path);
    /**
     * Send Post request to Host
     */
    HTTPResponseMessage *post(std::string path, std::string data, std::string type);
    /**
     * Read message from socket
     */
    HTTPMessage *read(int type);
    /**
     * Write message over socket
     */
    void write(HTTPMessage *message);
};
#endif