#include <string>

class ResourceRecord
{
private:
    string name;
    int16_t type;
    int16_t classCode;
    int32_t ttl;
    int16_t rdlength;
    char *rdata;

public:
    ResourceRecord(string name, int16_t type, int16_t classCode, int32_t ttl, int16_t rdlength, char *rdata);
    string getName();
    int16_t getType();
    int16_t getClassCode();
    int32_t getTTL();
    int16_t getRDLength();
    char *getRdata();
};
