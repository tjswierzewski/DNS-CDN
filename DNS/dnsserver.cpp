#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <string>
#include "DNSMessage.h"
#include "GeoCoordToDistance.h"
#include "CDNServer.h"

int main(int argc, char const *argv[])
{
    if (argc != 5)
    {
        std::cout << "Usage: ./dnsserver -p <port> -n <name>" << std::endl;
        exit(EXIT_FAILURE);
    }

    int udp_fd, udp_socket, valread;
    struct sockaddr_in address;
    struct sockaddr_in clientAddress;
    int opt = 1;
    int addrlen = sizeof(address);
    int clientAddrLen;
    char buffer[1024];
    std::string hello = "Hello from DNS Server";

    if ((udp_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        perror("UDP Socket failed");
        exit(EXIT_FAILURE);
    }

    memset((char *)&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(argv[2]));
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(udp_fd, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        perror("UDP Bind failed");
        exit(EXIT_FAILURE);
    }

// GEO LOCATION
// Get servers
std:
    std::map<int, CDNServer> serverList;
    std::string line;
    std::ifstream serversFile("servers.txt");
    if (serversFile.is_open())
    {
        while (getline(serversFile, line))
        {
            CDNServer server(line);
            serverList.insert(std::make_pair(server.getIP(), server));
        }
    }

    while (1)
    {
        int size = recvfrom(udp_fd, buffer, 1024, 0, (sockaddr *)&clientAddress, (socklen_t *)&clientAddrLen);
        DNSMessage query(buffer, size);
        DNSQuestion *question;
        if (question = query.getQuestion(), question && question->getName().compare(argv[4]) == 0)
        {
            std::cout << "I know that one" << std::endl;
            DNSMessage response(query.getIdentification(), 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0);
            response.addQuestion(*question);
            std::string ip = "\x8B\x90\x1E\x19";
            DNSResponse answer(question->getName(), 1, 1, 10, 4, ip);
            response.addAnswer(answer);
            std::string message = response.format();
            sendto(udp_fd, message.c_str(), message.size(), 0, (sockaddr *)&clientAddress, clientAddrLen);
        }
        else
        {
            std::cout << "I don't know that one" << std::endl;
        }
    }

    return 0;
}
