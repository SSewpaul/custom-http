#include <string>
#include <iostream>
#include <vector>

enum Method {
    GET,
    HEAD,
    POST,
};

class RequestLine {
    public:
        Method method;
        std::string request_uri;

        RequestLine() = default;

        RequestLine(std::string line) {
            std::string delimeter = " ";
            size_t pos = 0;
            std::string token;

            if (line.find("HTTP/1.0") == std::string::npos) {
                std::cerr << "Wrong HTTP version. Expected HTTP/1.0. Received: " << line.substr(line.size()-9, line.size()-5) << std::endl;
                return;
            }

            // Extract method from string
            if ((pos = line.find(delimeter)) == std::string::npos) {
                std::cerr << "Cannot process header request line" << std::endl;
                return;
            }

            token = line.substr(0, pos);

            if (token == "GET") {
                method = GET;
            }
            else if (token == "HEAD") {
                method = HEAD;
            }
            else if (token == "POST") {
                method = POST;
            }
            else {
                std::cerr << "Invalid method" << std::endl;
                return;
            }

            line.erase(0, pos + 1);

            if ((pos = line.find(delimeter)) == std::string::npos) {
                std::cerr << "Cannot process header request line" << std::endl;
                return;
            }

            request_uri = line.substr(0, pos);
        }
};

class ReqHeader {
    public:
        RequestLine request_line;

        ReqHeader() = default;

        ReqHeader(std::vector<std::string> header) {
            request_line = RequestLine(header[0]);
        }
};