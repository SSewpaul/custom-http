#include "reqheader.cpp"
#include <string>
#include <iostream>

class Request {
    public:
        ReqHeader header;
        std::vector<std::string> body;

        Request(std::vector<std::string> request_header, std::vector<std::string> request_body)
        {
            header = ReqHeader(request_header);
            body = request_body;
        }
};