#include <map>
#include <string>
#include <vector>
#include "http_time.cpp"
#include "method.h"

const static std::string VALID_HEADERS[] = {
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


class RequestSerializer {
    public:
        std::map<std::string, std::string> header;
        std::string body;
        Method method;
        std::string uri;
        std::string http_version;

    RequestSerializer() = default;

    RequestSerializer(const std::map<std::string, std::string> &reqheader, const std::string &reqbody, const std::string & reqstatus) {
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

    bool validate_header() {

    }
};