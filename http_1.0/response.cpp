#include <string>
#include <iostream>
#include <map>
#include <sys/socket.h>
#include "method.h"
#include "http_time.cpp"

class Response
{
public:
    std::map<std::string, std::string> headers;
    std::string body;
    std::string http_version;
    std::string reason_phrase;
    int status_code;

    Response() = default;

    Response(std::map<std::string, std::string> headers, std::string body, std::string http_version, std::string reason_phrase, int status_code) : 
    headers(headers), body(body), http_version(http_version), reason_phrase(reason_phrase), status_code(status_code) {}

    void send_response(int& fd) {
        std::string message = "";
        headers.insert({"Content-Length", std::to_string(body.length())});
        format_response(message);

        if (send(fd, (void *)message.c_str(), message.length(), 0) == -1) {
            std::cerr << "Cannot send message" << std::endl;
        }
    }

    void format_response(std::string &message) {
        message += (http_version + " " + std::to_string(status_code) + " " + reason_phrase + "\r\n");

        for (auto iter = headers.begin(); iter != headers.end(); iter++) {
            message += (iter->first + ": " + iter->second + "\r\n");
        }

        message += "\r\n";

        message += body;
    }
};