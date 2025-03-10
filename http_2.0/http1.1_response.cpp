#include <map>
#include <string>
#include <iostream>
#include <sys/socket.h>

class HTTP1Response
{
public:
    std::map<std::string, std::string> header;
    std::string body;
    std::string reason;
    unsigned int status_code;
    int sockfd;

    HTTP1Response() = default;

    HTTP1Response(std::map<std::string, std::string> &header, std::string &body, std::string &reason,
                  unsigned int &status_code, int sockfd) : header(header), body(body), reason(reason), status_code(status_code), sockfd(sockfd) {}

    void send_response()
    {
        std::string message = "";
        header.insert({"Content-Length", std::to_string(body.length())});
        format_response(message);

        if (send(sockfd, (void *)message.c_str(), message.length(), 0) == -1)
        {
            std::cerr << "Cannot send message" << std::endl;
        }
    }

    void format_response(std::string &message)
    {
        message += ("HTTP/1.1 " + std::to_string(status_code) + " " + reason + "\r\n");

        for (auto iter = header.begin(); iter != header.end(); iter++)
        {
            message += (iter->first + ": " + iter->second + "\r\n");
        }

        message += "\r\n";

        message += body;
    }
};