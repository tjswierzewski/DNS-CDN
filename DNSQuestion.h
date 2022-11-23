#include <string>

class DNSQuestion
{
private:
    char nameLength;
    std::string name;
    unsigned short int qType;
    unsigned short int qClass;
    void readName(char *message);

public:
    DNSQuestion(char *message);
    int getNameLength();
    std::string getName();
    int getQType();
    int getQClass();
    int getMessageSize();
    std::string format();
};