#include <string>

class DNSResponse
{
private:
    char nameLength;
    std::string name;
    unsigned short int type;
    unsigned short int dnsClass;
    unsigned int ttl;
    unsigned short int dataLength;
    vector < ;

public:
    DNSResponse(std::string name, unsigned short int type, unsigned short int dnsClass, unsigned int ttl, unsigned short int, );
    ~DNSResponse();
};