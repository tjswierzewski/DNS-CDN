#ifndef HTTPSSESSION_H
#define HTTPSSESSION_H

#include <map>
#include <string>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "HTTPResponseMessage.h"
#include "HTTPRequestMessage.h"

class HTTPSSession
{
public:
    typedef std::map<std::string, std::string> cookiesMap;

private:
    cookiesMap cookies;
    int socket;
    const char *host, *port;
    bool https;
    SSL_CTX *ctx;
    SSL *ssl;

    /**
     * Create socket connection with Host
     */
    int connectToHost();
    /**
     * Initialize SSL certificate
     */
    SSL_CTX *InitCTX(void);
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
    HTTPSSession(const char *host, const char *port);
    /**
     * Delete HTTP Session object
     */
    ~HTTPSSession();
    /**
     * Send Get request to Host
     */
    HTTPResponseMessage get(std::string path);
    /**
     * Send Post request to Host
     */
    HTTPResponseMessage post(std::string path, std::string data, std::string type);
};
#endif