#include "CDNServer.h"
#include <arpa/inet.h>

/**
 * Create empty CDN Server
 */
CDNServer::CDNServer()
{
}
/**
 * Create CDN Server with provided IP
 */
CDNServer::CDNServer(std::string ip)
{
    (inet_pton(AF_INET, ip.c_str(), &this->ip));
    this->ip = htonl(this->ip);
}
/**
 * Return server IP
 */
int CDNServer::getIP() const
{
    return this->ip;
}
/**
 * Less than comparison
 */
bool operator<(const CDNServer &s1, const CDNServer &s2)
{
    return s1.getIP() < s2.getIP();
}
/**
 * More than comparison
 */
bool operator>(const CDNServer &s1, const CDNServer &s2)
{
    return s1.getIP() > s2.getIP();
}
/**
 * Set Latitude of server
 */
void CDNServer::setLatitude(long double latitude)
{
    this->latitude = latitude;
}
/**
 * Set Longitude of server
 */
void CDNServer::setLongitude(long double longitude)
{
    this->longitude = longitude;
}
/**
 * Return byte string of IP for network
 */
std::string CDNServer::networkIP() const
{
    int temp = ntohl(this->ip);
    std::string out((char *)&temp, 4);
    return out;
}
/**
 * Get server latitude
 */
long double CDNServer::getLatitude() const
{
    return this->latitude;
}
/**
 * Get server longitude
 */
long double CDNServer::getLongitude() const
{
    return this->longitude;
}