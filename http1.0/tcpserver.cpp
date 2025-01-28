#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT "8080"
#define BACKLOG 10

class TCPServer
{
    public:
        int sockfd;

    private:
        struct addrinfo hints, *servinfo, *p;
        int status, connectedsockfd;
        struct sockaddr_storage their_addr; // connector's address information
        socklen_t sin_size;
        std::string s;

    public:
        int start_server()
        {
            // Get host address info
            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_flags = AI_PASSIVE;

            if ((status = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
            {
                std::cerr << "Error getting host info: " << gai_strerror(status) << std::endl;
                return 1;
            }

            // Loop through linked list and attempt to bind to a socket
            for (p = servinfo; p != nullptr; p = p->ai_next)
            {
                if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
                {
                    std::cerr << "Error creating socket" << std::endl;
                    continue;
                }

                if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
                {
                    close(sockfd);
                    std::cerr << "Error binding socket" << std::endl;
                    continue;
                }

                break;
            }

            freeaddrinfo(servinfo); // Free linked list

            if (p == nullptr)
            {
                std::cerr << "Could not bind" << gai_strerror(status) << std::endl;
                return 1;
            }

            // Start listening on port
            if (listen(sockfd, BACKLOG) == -1)
            {
                std::cerr << "Error listening" << gai_strerror(status) << std::endl;
                return 1;
            }

            while (1)
            {
                sin_size = sizeof their_addr;
                connectedsockfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

                if (connectedsockfd == -1)
                {
                    std::cerr << "Error accepting connection" << gai_strerror(status) << std::endl;
                    ;
                    continue;
                }

                std::cout << "Connection accepted" << std::endl;

                close(connectedsockfd);
            }

            return 0;
        }

        void stop_server() {
            close(connectedsockfd);
            close(sockfd);
        }
};