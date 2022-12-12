#include <unistd.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <arpa/inet.h>
#include <set>
#include <map>
#include <list>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <string>
#include <float.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
#include "DNSMessage.h"
#include "GeoCoordToDistance.h"
#include "CDNServer.h"
#include "IPLocation.h"
#include "../JSON/JSON.h"
#include "../JSON/JSONString.h"
#include "../JSON/JSONJson.h"

struct serverStats
{
    const CDNServer *server;
    double ping;
    int ttl;
};

bool checkIP(IPLocation it, int ip)
{
    return (it.getStartIP() < ip) && (it.getEndIP() > ip);
}

std::list<JSON> parseScamperJson(std::string response)
{
    std::list<JSON> jsonObjects;
    int lineStart = 0;
    int newLine = 0;
    int nextOpenBracket = -1;
    std::string jsonString;
    int openQuoteKey = 0;
    int comma = 0;
    int colon = 0;
    while ((newLine = response.find('\n', lineStart + 1)) != std::string::npos)
    {
        jsonString = response.substr(lineStart, newLine - lineStart);
        lineStart = newLine + 1;
        jsonObjects.emplace_back(JSON(jsonString));
    }
    return jsonObjects;
}
int main(int argc, char const *argv[])
{
    if (argc != 5)
    {
        std::cout << "Usage: ./dnsserver -p <port> -n <name>" << std::endl;
        exit(EXIT_FAILURE);
    }

    // scamper
    std::list<std::string> remotes;
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if ((dp = opendir("../remotes")) == NULL)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dp)) != NULL)
    {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
        {

            std::string path("../remotes/");
            path.append(entry->d_name);
            remotes.push_back(path);
        }
    }

    auto remoteIter = remotes.begin();

    // GEO LOCATION
    // Load Geo Location
    std::set<IPLocation>
        IPLocations;
    std::string line;
    std::ifstream locationFile("../IP2LOCATION/IPLocation.csv");
    if (locationFile.is_open())
    {
        while (getline(locationFile, line))
        {
            IPLocations.emplace(IPLocation(line));
        }
    }
    locationFile.close();
    // Get servers
    std::set<CDNServer> serverList;
    std::map<unsigned int, struct serverStats> matchedServer;
    std::ifstream serversFile("servers.txt");
    if (serversFile.is_open())
    {
        while (getline(serversFile, line))
        {
            CDNServer server(line);
            auto location = std::prev(std::lower_bound(IPLocations.begin(), IPLocations.end(), server.getIP()));
            server.setLatitude(location->getLatitude());
            server.setLongitude(location->getLongitude());
            serverList.insert(server);
        }
    }
    serversFile.close();

    int udp_fd, udp_socket, valread;
    struct sockaddr_in address;
    struct sockaddr_in clientAddress;
    int opt = 1;
    int addrlen = sizeof(address);
    int clientAddrLen = sizeof(address);
    char buffer[1024];

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
    while (1)
    {
        // DNS
        int size = recvfrom(udp_fd, buffer, 1024, 0, (sockaddr *)&clientAddress, (socklen_t *)&clientAddrLen);
        DNSMessage query(buffer, size);
        DNSQuestion *question;
        if (question = query.getQuestion(), question && question->getName().compare(argv[4]) == 0)
        {
            std::cout << "I know that one" << std::endl;
            DNSMessage response(query.getIdentification(), 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0);
            response.addQuestion(*question);
            unsigned int clientIP = htonl(clientAddress.sin_addr.s_addr);
            struct serverStats optimalServer;
            if (auto it = matchedServer.find(clientIP); it == matchedServer.end())
            {
                auto location = std::prev(std::lower_bound(IPLocations.begin(), IPLocations.end(), clientIP));
                const CDNServer *chosenServer;
                long double optimalDistance = 100000;
                for (auto &&server : serverList)
                {
                    long double serverDistance = GeoCoordToDistance::toMiles(server.getLatitude(), server.getLongitude(), location->getLatitude(), location->getLongitude());
                    if (serverDistance < optimalDistance)
                    {
                        chosenServer = &server;
                        optimalDistance = serverDistance;
                    }
                }
                struct serverStats optimal;
                optimal.server = chosenServer;
                optimal.ping = DBL_MAX;
                optimal.ttl = 10;
                matchedServer.insert({clientIP, optimal});
                optimalServer = optimal;
            }
            else
            {
                optimalServer = it->second;
            }
            DNSResponse answer(question->getName(), 1, 1, optimalServer.ttl, 4, optimalServer.server->networkIP());
            response.addAnswer(answer);
            std::string message = response.format();
            sendto(udp_fd, message.c_str(), message.size(), 0, (sockaddr *)&clientAddress, clientAddrLen);
        }
        else
        {
            std::cout << "I don't know that one" << std::endl;
        }

        // SCAMPER
        int commandPipe[2];
        int resultsPipe[2];
        pipe(commandPipe);
        pipe(resultsPipe);

        if (remoteIter == remotes.end())
        {
            remoteIter = remotes.begin();
        }

        int pid = fork();
        if (pid == 0)
        {
            dup2(commandPipe[0], STDIN_FILENO);
            dup2(resultsPipe[1], STDOUT_FILENO);
            ::close(commandPipe[1]);
            ::close(resultsPipe[0]);
            const char *exec[] = {"./scamperScript",
                                  remoteIter->c_str(),
                                  NULL};
            execvp(exec[0], (char *const *)exec);
            exit(EXIT_FAILURE);
        }
        int status;
        ::close(commandPipe[0]);
        ::close(resultsPipe[1]);
        std::string command("ping 8.8.8.8\n");
        ::write(commandPipe[1], command.c_str(), command.length());
        ::close(commandPipe[1]);
        waitpid(pid, &status, 0);
        char scamperBuffer[5000];
        int scamperRC = read(resultsPipe[0], scamperBuffer, 5000);
        ::close(resultsPipe[0]);
        std::list<JSON> measurements = parseScamperJson(std::string(scamperBuffer, scamperRC));
        for (auto &&json : measurements)
        {
            JSONString *type = (JSONString *)json.getData().at("type");
            if (type->getValue().compare("ping") == 0)
            {
                int srcAddr, dstAddr;
                JSONString *src = (JSONString *)json.getData().at("src");
                JSONString *dst = (JSONString *)json.getData().at("dst");
                inet_pton(AF_INET, src->getValue().c_str(), &srcAddr);
                inet_pton(AF_INET, dst->getValue().c_str(), &dstAddr);
                JSONJson *statistics = (JSONJson *)json.getData().at("statistics");
                JSONJson *avg = (JSONJson *)statistics->getData().at("avg");
                struct serverStats *stats = &matchedServer.find(dstAddr)->second;
                if (stats->server->getIP() == srcAddr)
                {
                    stats->ping == avg->getValue();
                    stats->ttl *= 2;
                }
                else
                {
                    if (stats->ping > avg->getValue())
                    {
                        stats->server =
                    }
                }
            }
        }
    }

    return 0;
}
