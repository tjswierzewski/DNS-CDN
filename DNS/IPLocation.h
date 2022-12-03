#include <string>
#include <vector>

class IPLocation
{
private:
    unsigned int startIP;
    unsigned int endIP;
    std::string countryAbbreviation;
    std::string country;
    std::string region;
    std::string city;
    long double latitude;
    long double longitude;

    static std::vector<std::string> splitRow(std::string str, std::string delimiter);

public:
    IPLocation(long startIP, long endIP, long double latitude, long double longitude);
    IPLocation(std::string line);
    int getStartIP() const;
    int getEndIP() const;
    long double getLatitude() const;
    long double getLongitude() const;
};
bool operator<(const IPLocation &l1, const IPLocation &l2);
bool operator<(const IPLocation &l1, const long &ip);
bool operator>(const IPLocation &l1, const IPLocation &l2);
bool operator>(const IPLocation &l1, const long &ip);
