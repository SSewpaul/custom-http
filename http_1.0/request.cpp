#include <map>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include "http_time.cpp"
#include "method.h"

static std::string VALID_HEADERS[] = {
    "Date",
    "Pragma",
    "Authorization",
    "From",
    "If-Modified-Since",
    "Referer",
    "User-Agent",
    "Allow",
    "Content-Encoding",
    "Content-Length",
    "Content-Type",
    "Expires",
    "Last-Modified",
};

class Request {
    public:
        std::map<std::string, std::string> headers;
        std::string body;
        Method method;
        std::string uri;
        std::string http_version;

    Request() = default;

    Request(std::map<std::string, std::string> &reqheaders, const std::string &reqbody, const std::string & reqstatus) {
        parse_status_line(reqstatus);
        headers = reqheaders;
        validate_header();
    }

    void validate_header() {
        for (auto iter = headers.begin(); iter != headers.end();) {
            std::string* header = std::find(std::begin(VALID_HEADERS), std::end(VALID_HEADERS), iter->first);
            if (header == std::end(VALID_HEADERS)) {
                headers.erase(iter++);
            }
            else {
                iter++;
            }
        }
    }

    void parse_status_line(const std::string & reqstatus) {
        size_t m, n;

        if ((m = reqstatus.find(" ")) == std::string::npos) {
            std::cerr << "Invalid status line" << std::endl;
            return;
        }

        if ((n = reqstatus.find(" ", m+1)) == std::string::npos) {
            std::cerr << "Invalid status line" << std::endl;
            return;
        }

        std::string strmethod = reqstatus.substr(0, m);
        method = get_method(strmethod);

        if (method == Method::ERROR) {
            std::cerr << "Cannot get status code" << std::endl;
            return;
        }

        uri = reqstatus.substr(m+1, n-(m+1));
        http_version = reqstatus.substr(n+1, reqstatus.length() - (n+1));
    }
};