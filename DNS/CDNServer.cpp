#include "CDNServer.h"
#include <arpa/inet.h>

CDNServer::CDNServer(std::string ip)
{
    (inet_pton(AF_INET, ip.c_str(), &this->ip));
    this->ip = htonl(this->ip);
}
int CDNServer::getIP()
{
    return this->ip;
}