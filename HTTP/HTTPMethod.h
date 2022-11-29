#ifndef HTTP_METHOD_H
#define HTTP_METHOD_H

#include <string>

class HTTPMethod
{
public:
    static std::string Get;
    static std::string Post;
    static std::string Patch;
    static std::string Delete;
    static std::string Put;
    static std::string Head;
    static std::string Options;
    static std::string Connect;
    static std::string Trace;
};
#endif