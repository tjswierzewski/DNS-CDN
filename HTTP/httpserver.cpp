#include <sys/epoll.h>
#define MAX_EVENTS 10
#define MAX_LOG 10

void handleRequest(int fd)
{
}

int main(int argc, char const *argv[])
{
    if (argc != 5)
    {
        std::cout << "Usage: ./dnsserver -p <port> -o <origin>" << std::endl;
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        struct epoll_event ev, events[MAX_EVENTS];
        struct sockaddr_in clientAddress;
        int listen_sock, conn_sock, nfds, epollfd, clientAddrLength;

        epollfd = epoll_create1(0);
        if (epollfd == -1)
        {
            perror("epoll_create1");
            exit(EXIT_FAILURE);
        }

        listen_sock = socket(AF_INET, SOCK_STREAM, 0);

        if (listen(listen_sock, MAX_LOG) != 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        ev.events = EPOLLIN;
        ev.data.fd = listen_sock;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock.& ev) == -1)
        {
            perror("epoll_ctl: listen_sock");
            exit(EXIT_FAILURE);
        }

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
                if (events[n].data.fd == listen_sock)
                {
                    conn_sock = accept(listen_sock, (struct sockaddr *)&clientAddress, &clientAddrLength);
                    if (conn_sock == -1)
                    {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }
                    setnonblocking(conn_sock);
                    ev.events = EPOLLING | EPOLLET;
                    ev.data.fd = conn_sock;
                    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1)
                    {
                        perror("epoll_ctl: conn_sock");
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    handleRequest(fd);
                }
            }
        }
    }

    return 0;
}
