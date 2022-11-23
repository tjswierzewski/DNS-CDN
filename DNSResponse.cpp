#include <sstream>
#include <string>
#include "DNSResponse.h"

DNSResponse::DNSResponse(std::string name, unsigned short int type, unsigned short int dnsClass, unsigned int ttl, unsigned short int, std::string data)
{
    this->name = name;
    this->type = type;
    this->dnsClass = dnsClass;
    this->ttl = ttl;
    this->dataLength = dataLength;
    this->data = data;
}
std::string DNSResponse::format()
{
    std::string output;
    std::string token;
    int pos = 0;
    int delPos;
    output.push_back((char)192);
    output.push_back((char)12);
    output.push_back((char)(this->type >> 8));
    output.push_back((char)(this->type & 255));
    output.push_back((char)(this->dnsClass >> 8));
    output.push_back((char)(this->dnsClass & 255));
    output.push_back((char)(this->ttl >> 24));
    output.push_back((char)((this->ttl >> 16) & 255));
    output.push_back((char)((this->ttl >> 8) & 255));
    output.push_back((char)(this->ttl & 255));
    output.push_back((char)(this->dataLength >> 8));
    output.push_back((char)(this->dataLength & 255));
    output += this->data;
    return output;
}
