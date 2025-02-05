#include <string>
#include <ctime>

static const std::string REQUEST_HEADER_FIELDS[] = {"Authorization", "From", "If-Modified-Since", "Referer", "User-Agent"};

class RequestHeader
{
public:
    std::string authorization;
    std::string from;
    HTTPTime if_modified_since;
    std::string referer;
    std::string user_agent;

    RequestHeader() : authorization(""), from(""), if_modified_since(HTTPTime()), referer(""), user_agent("") {}

    bool update_request_header(std::string field, std::string value) {
        if (field == "Authorization" && authorization.empty()) {
            // Add credentials logic later for authentication
            authorization = value;
            return true;
        }
        else if (field == "From" && from.empty()) {
            // Add phrase route-addr (name & addr-spec) later from RFC 822
            int pos = 0;
            if ((pos = value.find("@")) == std::string::npos) {
                return false;
            }

            if (value.substr(pos + 1).find(".") == std::string::npos) {
                return false;
            }

            from = value;
            return true;
        }
        else if (field == "If-Modified-Since" && !if_modified_since.valid) {
            if (if_modified_since.update_time(value)) {
                return true;
            }
            else {
                return false;
            }
        }
        else if (field == "Referer" && referer.empty()) {
            referer = value;
            return true;
        }
        else if (field == "User-Agent" && user_agent.empty()) {
            user_agent = value;
            return true;
        }

        return false;
    }
};