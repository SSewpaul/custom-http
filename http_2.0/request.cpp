#include <map>
#include <string>

class Request
{
public:
    std::map<std::string, std::string> header;
    std::string body;
    std::string status_line;

    Request() = default;

    Request(const char (&buf)[1024], const int& numbytes) {
        process_request(buf, header, body, status_line, numbytes);
    }

    void process_request(const char (&buf)[1024], std::map<std::string, std::string> &header, std::string &body, std::string &status_line, const int &numbytes)
    {
        std::string curr_line;
        int i;

        // Process headers into a map
        for (i = 0; i < numbytes + 1; i++)
        {
            if (buf[i] == '\r' && buf[i + 1] == '\n')
            {
                curr_line += '\0';

                if (curr_line.length() > 0)
                {
                    size_t pos = curr_line.find(": ");

                    if (pos == std::string::npos && curr_line.find("HTTP/1.1") != std::string::npos)
                    {
                        status_line = curr_line;
                    }
                    else
                    {
                        std::string key = curr_line.substr(0, pos);
                        std::string val = curr_line.substr(pos + 2, curr_line.length());
                        header.insert({key, val});
                    }
                }

                // Check if we are at the end of header
                if (buf[i + 2] == '\r' && buf[i + 3] == '\n')
                {
                    i += 3;
                    break;
                }
                else
                {
                    i += 1;
                }

                curr_line.clear();
            }
            else
            {
                curr_line += buf[i];
            }
        }

        // Process body
        for (; i < numbytes + 1; i++)
        {
            body += buf[i];
        }

        body += '\0';
    }

    bool is_http2_upgrade() {
        if (header.count("Upgrade") > 0 && header.at("Upgrade").compare("h2c") == 0) {
            return true;
        }

        return false;
    }
};