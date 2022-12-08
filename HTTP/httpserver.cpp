#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/epoll.h>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include "HTTPSession.h"
#include "HTTPRequestMessage.h"
#include "HTTPResponseMessage.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#define MAX_EVENTS 10
#define MAX_LOG 10

int checkCache(std::string path)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if ((dp = opendir("../cache")) == NULL)
    {
        perror("opendir");
        return -1;
    }

    while ((entry = readdir(dp)) != NULL)
    {
        lstat(entry->d_name, &statbuf);
        if (path.compare(entry->d_name) == 0)
        {
            return 0;
        }
    }
    return -1;
}

int main(int argc, char const *argv[])
{
    if (argc != 5)
    {
        std::cout << "Usage: ./dnsserver -p <port> -o <origin>" << std::endl;
        exit(EXIT_FAILURE);
    }
    struct epoll_event ev, events[MAX_EVENTS];
    struct sockaddr_in clientAddress;
    struct sockaddr_in selfAddress;
    int listen_sock, conn_sock, nfds, epollfd, clientAddrLength;

    epollfd = epoll_create1(0);
    if (epollfd == -1)
    {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    listen_sock = socket(AF_INET, SOCK_STREAM, 0);

    selfAddress.sin_family = AF_INET;
    selfAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    selfAddress.sin_port = htons(atoi(argv[2]));

    if (bind(listen_sock, (struct sockaddr *)&selfAddress, sizeof(selfAddress)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(listen_sock, MAX_LOG) != 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1)
    {
        perror("epoll_ctl: listen_sock");
        exit(EXIT_FAILURE);
    }

    std::map<int, HTTPSession> openSessions;

    while (1)
    {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1)
        {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        for (size_t i = 0; i < nfds; i++)
        {
            if (events[i].events == EPOLLIN)
            {
                if (events[i].data.fd == listen_sock)
                {
                    try
                    {
                        HTTPSession session(listen_sock);
                        int conn_sock = session.getFD();
                        openSessions.insert({conn_sock, session});
                        ev.events = EPOLLIN | EPOLLET;
                        ev.data.fd = conn_sock;
                        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1)
                        {
                            perror("epoll_ctl: conn_sock");
                            exit(EXIT_FAILURE);
                        }
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << e.what() << '\n';
                        continue;
                    }
                }
                else
                {
                    HTTPSession session = openSessions.at(events[i].data.fd);
                    HTTPRequestMessage *request = (HTTPRequestMessage *)session.read(HTTP_REQUEST);
                    if (request)
                    {
                        if (request->getPath().compare("/grading/beacon") == 0)
                        {
                            HTTPMessage::headerMap headers;
                            HTTPResponseMessage response = HTTPResponseMessage(request->getVersion(), 204, "No Content", headers);
                            session.write(&response);
                        }
                        else
                        {
                            // request page from origin
                            std::string path = request->getPath();
                            path.erase(0, 1);
                            if (path.compare("") == 0)
                            {
                                path = "index.html";
                            }
                            if (checkCache(path) > -1)
                            {
                                std::ifstream file(std::string("../cache/").append(path));
                                std::stringstream buffer;
                                HTTPMessage::headerMap headers;
                                buffer << file.rdbuf();
                                file.close();
                                HTTPResponseMessage response = HTTPResponseMessage(request->getVersion(), 200, "OK", headers);
                                response.setData(buffer.str());
                                std::cout << "THIS IS CACHED" << std::endl;
                                session.write(&response);
                            }
                            else
                            {
                                HTTPSession origin(argv[4], "8080");
                                HTTPResponseMessage *page = origin.get(request->getPath());
                                // send page to client
                                session.write(page);
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}
