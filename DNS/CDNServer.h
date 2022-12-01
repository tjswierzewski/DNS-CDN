#include <string>

class CDNServer
{
private:
    unsigned int ip;
    long double latitude;
    long double longitude;

public:
    CDNServer();
    CDNServer(std::string ip);
    int getIP() const;
    void setLatitude(long double latitude);
    void setLongitude(long double longitude);
};
bool operator<(const CDNServer &s1, const CDNServer &s2);
bool operator>(const CDNServer &s1, const CDNServer &s2);
