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
#include "request.cpp"

#define PORT "8080"
#define BACKLOG 10

class TCPServer
{
    public:
        int sockfd;

    private:
        struct addrinfo hints, *servinfo, *p;
        int status;
        std::string s;
        const int opt = 1;

    public:

        /**
         * Starts TCP server on port 8080
         */
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
                return -1;
            }

            // Loop through linked list and attempt to bind to a socket
            for (p = servinfo; p != nullptr; p = p->ai_next)
            {
                if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
                {
                    std::cerr << "Error creating socket" << std::endl;
                    continue;
                }

                if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1) {
                    perror("setsockopt");
                    return 1;
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
                std::cerr << "Could not bind" << std::endl;
                return -1;
            }

            // Start listening on port
            if (listen(sockfd, BACKLOG) == -1)
            {
                std::cerr << "Error listening" << std::endl;
                return -1;
            }

            return sockfd;
        }

        ~TCPServer() {
            close(sockfd);
        }
};