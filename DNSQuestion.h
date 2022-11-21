#include <string>

class DNSQuestion
{
private:
    char nameLength;
    std::string name;
    unsigned short int qType;
    unsigned short int qClass;

public:
    DNSQuestion(char *message);
    int getNameLength();
    std::string getName();
    int getQType();
    int getQClass();
    int getMessageSize();
};