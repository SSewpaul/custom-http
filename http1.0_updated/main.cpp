#include <iostream>
#include <vector>
#include <map>
#include "tcp_server.cpp"
#include "request.cpp"
#include "response.cpp"

#define MAXDATASIZE 1024

/**
 * Process request and separate header and body
 *
 * @param buf Buffer that holds incoming HTTP request
 * @param header Holds resulting header after processing
 * @param body Holds resulting body after processing
 * @param numbytes Specifies the number of bytes of data stored in buffer
 */
void process_request(const char (&buf)[MAXDATASIZE], std::map<std::string, std::string> &header, std::string &body, std::string &status_line, const int &numbytes)
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

                if (pos == std::string::npos && curr_line.find("HTTP/1.0") != std::string::npos) {
                    status_line = curr_line;
                }
                else {
                    std::string key = curr_line.substr(0, pos);
                    std::string val = curr_line.substr(pos + 2, curr_line.length());
                    header.insert({key, val});
                }
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

            curr_line.clear();
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

        std::map<std::string, std::string> header;
        std::string body;
        std::string status_line;

        process_request(buf, header, body, status_line, numbytes);
        Request req(header, body, status_line);

        if (req.http_version != "HTTP/1.0") {
            std::cerr << "Invalid HTTP version. Received: " << req.http_version << std::endl;
        }

        if (req.method == GET) {
            Response res;
            res.headers.insert({"Content-Type", "application/json"});
            res.body = "{test: \"test\"}";
            res.http_version = "HTTP/1.0";
            res.status_code = 200;
            res.reason_phrase = "OK";

            res.send_response(clientsockfd);
        }

        close(clientsockfd);
    }
}