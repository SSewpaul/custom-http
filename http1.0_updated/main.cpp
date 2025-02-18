#include <iostream>
#include <vector>
#include <map>
#include "tcp_server.cpp"

#define MAXDATASIZE 1024

/**
 * Process request and separate header and body
 *
 * @param buf Buffer that holds incoming HTTP request
 * @param header Holds resulting header after processing
 * @param body Holds resulting body after processing
 * @param numbytes Specifies the number of bytes of data stored in buffer
 */
void process_request(const char (&buf)[MAXDATASIZE], std::map<std::string, std::string> &header, std::string &body, const int &numbytes)
{
    std::string curr_line;
    int i;

    // Process headers into a map
    for (i = 0; i < numbytes + 1; i++)
    {
        if (buf[i] == '\r' && buf[i + 1] == '\n')
        {
            curr_line += '\0';

            if (curr_line.length() > 0)
            {
                size_t pos = curr_line.find(": ");
                std::string key = curr_line.substr(0, pos);
                std::string val = curr_line.substr(pos + 2, curr_line.length());
                header.insert({key, val});
            }

            // Check if we are at the end of header
            if (buf[i + 2] == '\r' && buf[i + 3] == '\n')
            {
                i += 3;
                break;
            }
            else
            {
                i += 1;
            }
        }
        else
        {
            curr_line += buf[i];
        }
    }

    // Process body
    for (; i < numbytes + 1; i++) {
        body += buf[i];
    }

    body += '\0';
}

int main()
{
    int connectedsockfd, sockfd, numbytes;
    socklen_t sin_size;
    struct sockaddr_storage their_addr; // connector's address information
    char buf[MAXDATASIZE];

    TCPServer tcp_server;
    if ((sockfd = tcp_server.start_server()) == -1)
    {
        std::cerr << "Cannot start server" << std::endl;
    }

    while (1)
    {
        // Accept connection
        sin_size = sizeof their_addr;
        connectedsockfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

        if (connectedsockfd == -1)
        {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        std::cout << "Connection accepted" << std::endl;

        // Receive HTTP packets
        if ((numbytes = recv(connectedsockfd, buf, MAXDATASIZE - 1, 0)) == -1)
        {
            std::cerr << "Error recieving data" << std::endl;
        }

        // std::cout << buf << std::endl;

        std::map<std::string, std::string> header;
        std::string body;

        process_request(buf, header, body, numbytes);

        close(connectedsockfd);
    }
}