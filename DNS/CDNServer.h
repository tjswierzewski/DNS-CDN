#include <string>

class CDNServer
{
private:
    unsigned int ip;
    long double latitude;
    long double longitude;

public:
    CDNServer(std::string ip);
    int getIP();
};