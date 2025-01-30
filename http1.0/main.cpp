#include "tcpserver.cpp"
#include <iostream>
#include <vector>

#define MAXDATASIZE 1024

/**
 * Process request and separate header and body
 * 
 * @param buf Buffer that holds incoming HTTP request
 * @param header Holds resulting header after processing
 * @param body Holds resulting body after processing
 * @param numbytes Specifies the number of bytes of data stored in buffer
 */
void process_request(const char (&buf)[MAXDATASIZE], std::vector<std::string> &header, std::vector<std::string> &body, const int &numbytes)
{
    std::string curr_line;
    bool is_header = true;

    for (int i = 0; i < numbytes + 1; i++)
    {
        // Check if we are at the end of the line
        if (buf[i] == '\r' && buf[i + 1] == '\n')
        {
            curr_line += '\0';
            is_header ? header.push_back(curr_line) : body.push_back(curr_line);
            curr_line.clear();

            // If there are 2 CRLFs in a row end of header has been reached
            if (buf[i + 2] == '\r' && buf[i + 3] == '\n')
            {
                is_header = false;
                i += 3;
            }
            else
            {
                i += 1;
            }
        }
        else if (buf[i] != '\r' || buf[i] != '\n')
        {
            curr_line += buf[i];
        }
    }

    curr_line += '\0';
    if (curr_line != "\0")
    {
        curr_line += '\0';
        is_header ? header.push_back(curr_line) : body.push_back(curr_line);
    }
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

        std::cout << buf << std::endl;

        std::vector<std::string> header;
        std::vector<std::string> body;

        process_request(buf, header, body, numbytes);

        for (int i = 0; i < header.size(); i++) {
            std::cout << header[i] << std::endl;
        }

        close(connectedsockfd);
    }
}