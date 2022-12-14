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
    std::string networkIP() const;
    void setLatitude(long double latitude);
    void setLongitude(long double longitude);
    long double getLatitude() const;
    long double getLongitude() const;
};
bool operator<(const CDNServer &s1, const CDNServer &s2);
bool operator>(const CDNServer &s1, const CDNServer &s2);
