#include <iostream>
#include <vector>
#include <map>
#include "tcp_server.cpp"

#define MAXDATASIZE 1024

// get sockaddr, IPv4 or IPv6:
void* get_in_addr(struct sockaddr* sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

in_port_t get_port(struct sockaddr* sa)
{
    if (sa->sa_family == AF_INET) {
        return ((struct sockaddr_in*)sa)->sin_port;
    }

    return ((struct sockaddr_in6*)sa)->sin6_port;
}

int main()
{
    int clientsockfd, sockfd, numbytes;
    socklen_t sin_size;
    struct sockaddr_storage client_addr; // connector's client_addr_str address information
    char buf[MAXDATASIZE];
    char client_addr_str[INET6_ADDRSTRLEN];

    TCPServer tcp_server;
    if ((sockfd = tcp_server.start_server()) == -1)
    {
        std::cerr << "Cannot start server" << std::endl;
    }

    while (1)
    {
        // Accept connection
        sin_size = sizeof client_addr;
        clientsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);

        if (clientsockfd == -1)
        {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }
        
        inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *)&client_addr), client_addr_str, sizeof client_addr_str);

        std::cout << "Connection accepted from: " << client_addr_str << ":" << get_port((struct sockaddr *)&client_addr) << std::endl;

        // Receive HTTP packets
        if ((numbytes = recv(clientsockfd, buf, MAXDATASIZE - 1, 0)) == -1)
        {
            std::cerr << "Error recieving data" << std::endl;
        }

        std::cout << buf << std::endl;


        close(clientsockfd);
    }
}