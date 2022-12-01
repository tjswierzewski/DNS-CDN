#include <string>
#include <vector>
#include "IPLocation.h"

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
IPLocation::IPLocation(long startIP, long endIP, std::string countryAbbreviation, std::string country, std::string region, std::string city, long double latitude, long double longitude)
{
    this->startIP = startIP;
    this->endIP = endIP;
    this->countryAbbreviation = countryAbbreviation;
    this->country = country;
    this->region = region;
    this->city = city;
    this->latitude = latitude;
    this->longitude = longitude;
}
IPLocation::IPLocation(std::string line)
{
    std::vector<std::string> attributes = IPLocation::splitRow(line, "\",\"");
    this->startIP = std::stol(attributes[0]);
    this->endIP = std::stol(attributes[1]);
    this->countryAbbreviation = attributes[2];
    this->country = attributes[3];
    this->region = attributes[4];
    this->city = attributes[5];
    this->latitude = std::stold(attributes[6]);
    this->longitude = std::stold(attributes[7]);
}
bool operator<(const IPLocation &l1, const IPLocation &l2)
{
    return l1.getStartIP() < l2.getStartIP();
}
bool operator>(const IPLocation &l1, const IPLocation &l2)
{
    return l1.getStartIP() > l2.getStartIP();
}
bool operator<(const IPLocation &l1, const long &ip)
{
    return l1.getStartIP() < ip;
}
bool operator>(const IPLocation &l1, const long &ip)
{
    return l1.getStartIP() > ip;
}
int IPLocation::getStartIP() const
{
    return this->startIP;
}
int IPLocation::getEndIP() const
{
    return this->endIP;
}
long double IPLocation::getLatitude() const
{
    return this->latitude;
}
long double IPLocation::getLongitude() const
{
    return this->longitude;
}