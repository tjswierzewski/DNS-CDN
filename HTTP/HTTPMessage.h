#ifndef HTTP_MESSAGE_H
#define HTTP_MESSAGE_H

#define MAX_LENGTH 10000

#include <string>
#include <map>

class HTTPMessage
{
public:
    typedef std::multimap<std::string, std::string> headerMap;

protected:
    float version;
    headerMap headers;
    std::string data;
    /**
     * Create headers out of string
     */
    void parseHeaders(std::string *buffer);

public:
    /**
     * Create HTTP message out of buffer
     */
    HTTPMessage(std::string buffer);
    /**
     * Create HTTP message out of components
     */
    HTTPMessage(float version, headerMap headers, std::string data = "");
    /**
     * Deconstructor
     */
    virtual ~HTTPMessage();
    /**
     * Get HTTP version
     */
    float getVersion();
    /**
     * Get HTTP headers
     */
    headerMap getHeaders();
    /**
     * Get HTTP message data
     */
    std::string getData();
    /**
     * Get String of version
     */
    std::string printVersion();
    /**
     * Set a single header value
     */
    void setHeader(std::string key, std::string value);
    /**
     * Set message data
     */
    void setData(std::string data);
    /**
     * Return string of request formatted to be sent via socket
     */
    virtual std::string format();
    /**
     * Get length of data section
     */
    int getDataLength();
    /**
     * Remove a header
     */
    void removeHeader(std::string key);
};
#endif