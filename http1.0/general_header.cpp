#include <ctime>
#include <string>
#include <chrono>
#include <iostream>
#include "http_time.cpp"

static const std::string GENERAL_HEADER_FIELDS[2] = {"Date", "Pragma"};

class GeneralHeader
{
public:
    HTTPTime date;
    std::string pragma;

    GeneralHeader() {
        date = HTTPTime();
        pragma = "";
    };

    GeneralHeader(std::string strdate, std::string reqpragma)
    {
        date = HTTPTime(strdate);
        pragma = reqpragma;
    }

    int setValue(std::string field, std::string value)
    {
        if (field == "Date")
        {
            if (date.valid)
            {
                return -1;
            }

            if (date.update_time(value)) {
                return 0;
            }
        }
        else if (field == "Pragma" && pragma.empty())
        {
            pragma = value;
            return 0;
        }

        return -1;
    }
};