#include <iostream>
#include <vector>
#include <map>
#include "tcp_server.cpp"
#include "frame.cpp"
#include "http1.1_response.cpp"

#define MAXDATASIZE 1024

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

in_port_t get_port(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return ((struct sockaddr_in *)sa)->sin_port;
    }

    return ((struct sockaddr_in6 *)sa)->sin6_port;
}

void handleSwitching(int clientsockfd)
{
    std::map<std::string, std::string> header = {{"Upgrade", "h2c"},
                                                 {"Connection", "Upgrade"}};
    std::string body = "";
    unsigned int status_code = 101;
    std::string reason = "Switching Protocols";
    HTTP1Response res(header, body, reason, status_code, clientsockfd);

    // Send the upgrade response
    res.send_response();
    std::cout << "Upgrading to HTTP/2" << std::endl;
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

        std::string request(buf);
        std::cout << request << std::endl;

        if (request.find("Upgrade: h2c") != std::string::npos)
        {
            handleSwitching(clientsockfd);

            char prefaceBuffer[24];
            numbytes = read(clientsockfd, prefaceBuffer, 24);
            
            if (numbytes == 24) {
                std::string preface(prefaceBuffer);
                if (preface == "PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n") {
                    std::cout << "Received HTTP/2 preface. Ready to process frames.\n";
                }
            }
        }
        else
        {
            // If no upgrade request, send an error back
            std::map<std::string, std::string> header = {{"Content-Type", "text/plain"},
                                                         {"Connection", "close"}};
            std::string body = "This server does not support HTTP/1.1. Please use HTTP/2.";
            unsigned int status_code = 505;
            std::string reason = "HTTP Version Not Supported";
            HTTP1Response res(header, body, reason, status_code, clientsockfd);

            // Send the upgrade response
            res.send_response();
            std::cerr << "Wrong http version" << std::endl;
        }

        close(clientsockfd);
    }
}