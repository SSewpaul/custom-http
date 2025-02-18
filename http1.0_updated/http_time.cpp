#include <ctime>
#include <iostream>
#include <string>
#include <chrono>

#ifndef HTTP_TIME
#define HTTP_TIME

class HTTPTime
{
public:
    time_t date;
    struct tm tm;
    bool valid;

    HTTPTime()
    {
        auto t = std::chrono::system_clock::now();
        date = std::chrono::system_clock::to_time_t(t);
        valid = false;
    };

    HTTPTime(std::string strdate)
    {
        if (!update_time(strdate)) {
            std::cout << "Cannot initialize date" << std::endl;
        }
    }

    bool update_time(std::string strdate) {
        const char *status = strptime(strdate.c_str(), "%a, %d %b %Y %H:%M:%S GMT", &tm); // RFC 822 Format

        if (status == NULL)
        {
            status = strptime(strdate.c_str(), "%a, %d-%b-%y %H:%M:%S GMT", &tm); // RFC 850 Format

            if (status == NULL)
            {
                status = strptime(strdate.c_str(), "%a %b %d %H:%M:%S %Y", &tm); // ANSI C asctime() Format

                if (status == NULL)
                {
                    std::cerr << "Cannot process time" << std::endl;
                    return false;
                }
            }
        }

        date = mktime(&tm);
        valid = true;
        return true;
    }
};

#endif