#include <string>
#include <vector>
#include "IPLocation.h"

/**
 * Parse IP row
 */
std::vector<std::string> IPLocation::splitRow(std::string str, std::string delimiter)
{
    std::vector<std::string> parsed;
    std::string token;
    int pos = 1;
    int delPos;
    do
    {
        delPos = str.find(delimiter, pos);
        parsed.push_back(str.substr(pos, delPos - pos));
        pos = delPos + 3;
    } while (delPos != std::string::npos);
    return parsed;
}
/**
 * Create IP location entry
 */
IPLocation::IPLocation(long startIP, long endIP, long double latitude, long double longitude)
{
    this->startIP = startIP;
    this->endIP = endIP;
    this->latitude = latitude;
    this->longitude = longitude;
}
/**
 * Parse line from IP Location CSV
 */
IPLocation::IPLocation(std::string line)
{
    std::vector<std::string> attributes = IPLocation::splitRow(line, "\",\"");
    this->startIP = std::stol(attributes[0]);
    this->endIP = std::stol(attributes[1]);
    this->latitude = std::stold(attributes[2]);
    this->longitude = std::stold(attributes[3]);
}
/**
 * Less than comparator for IP location
 */
bool operator<(const IPLocation &l1, const IPLocation &l2)
{
    return l1.getStartIP() < l2.getStartIP();
}
/**
 * More than comparator for IP location
 */
bool operator>(const IPLocation &l1, const IPLocation &l2)
{
    return l1.getStartIP() > l2.getStartIP();
}
/**
 * Less than comparator for IP location against IP
 */
bool operator<(const IPLocation &l1, const long &ip)
{
    return l1.getStartIP() < ip;
}
/**
 * More than comparator for IP location against IP
 */
bool operator>(const IPLocation &l1, const long &ip)
{
    return l1.getStartIP() > ip;
}
/**
 * Get start of IP range
 */
int IPLocation::getStartIP() const
{
    return this->startIP;
}
/**
 * Get end of IP range
 */
int IPLocation::getEndIP() const
{
    return this->endIP;
}
/**
 * Get IP latitude
 */
long double IPLocation::getLatitude() const
{
    return this->latitude;
}
/**
 * Get IP longitude
 */
long double IPLocation::getLongitude() const
{
    return this->longitude;
}