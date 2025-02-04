#include "full_req_header.cpp"
#include <string>
#include <iostream>

class Request {
    public:
        FullRequestHeader header;
        std::vector<std::string> body;

        Request(std::vector<std::string> request_header, std::vector<std::string> request_body)
        {
            header = FullRequestHeader(request_header);
            body = request_body;
        }
};