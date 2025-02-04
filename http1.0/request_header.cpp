#include <string>

#define REQUEST_HEADER_FIELDS ["Authorization", "From", "If-Modified-Since", "Referer", "User-Agent"]


class RequestHeader {
    public:
        std::string authorization;
        std::string from;
        std::string if_modified_since;
        std::string referrer;
        std::string user_agent;

        RequestHeader() = default;
};