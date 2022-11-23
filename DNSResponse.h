#include <string>

class DNSResponse
{
private:
    std::string name;
    unsigned short int type;
    unsigned short int dnsClass;
    unsigned int ttl;
    unsigned short int dataLength;
    std::string data;

public:
    DNSResponse(std::string name, unsigned short int type, unsigned short int dnsClass, unsigned int ttl, unsigned short int, std::string data);
    std::string format();
};