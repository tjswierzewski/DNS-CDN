#include "CDNServer.h"
#include <arpa/inet.h>

CDNServer::CDNServer()
{
}
CDNServer::CDNServer(std::string ip)
{
    (inet_pton(AF_INET, ip.c_str(), &this->ip));
    this->ip = htonl(this->ip);
}
int CDNServer::getIP() const
{
    return this->ip;
}
bool operator<(const CDNServer &s1, const CDNServer &s2)
{
    return s1.getIP() < s2.getIP();
}
bool operator>(const CDNServer &s1, const CDNServer &s2)
{
    return s1.getIP() > s2.getIP();
}
void CDNServer::setLatitude(long double latitude)
{
    this->latitude = latitude;
}
void CDNServer::setLongitude(long double longitude)
{
    this->longitude = longitude;
}