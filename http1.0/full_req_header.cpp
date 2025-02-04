#include <string>
#include <iostream>
#include <vector>
#include <ctime>
#include "req_line.cpp"
#include "general_header.cpp"
#include "request_header.cpp"
#include "entity_header.cpp"

class FullRequestHeader
{
public:
    RequestLine request_line;
    std::vector<GeneralHeader> general_headers;
    std::vector<RequestHeader> request_headers;
    std::vector<EntityHeader> entity_headers;
    size_t num_general_headers, num_request_headers, num_entity_headers;

    FullRequestHeader()
    {
        num_general_headers = 0;
        num_request_headers = 0;
        num_entity_headers = 0;
    };

    FullRequestHeader(std::vector<std::string> header)
    {
        request_line = RequestLine(header[0]);
        num_general_headers = 0;
        num_request_headers = 0;
        num_entity_headers = 0;
        process_headers(header);
    }

private:
    bool check_and_process_general_headers(std::string &line)
    {
        int added_status = -2;
        for (auto field : GENERAL_HEADER_FIELDS)
        {
            std::string curr_value = line.substr(field.length() + 2);
            if (line.find(field + ":") != std::string::npos)
            {
                if (num_general_headers > 0 && (added_status = general_headers[num_general_headers - 1].setValue(field, curr_value)) == 0) {
                    return true;
                }
                else if (num_general_headers == 0 || (num_general_headers > 0 && added_status == -1))
                {
                    GeneralHeader h;
                    h.setValue(field, curr_value);
                    num_general_headers += 1;
                    general_headers.push_back(h);
                    return true;
                }
            }
        }

        return false;
    }

    void process_headers(std::vector<std::string> header)
    {
        for (auto it = header.begin() + 1; it != header.end(); it++)
        {
            if (check_and_process_general_headers(*it)) {
                continue;
            }
        }
    }
};