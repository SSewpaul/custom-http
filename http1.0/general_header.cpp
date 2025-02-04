#include <ctime>
#include <string>
#include <chrono>

static const std::string GENERAL_HEADER_FIELDS[2] = {"Date", "Pragma"};

class GeneralHeader
{
public:
    time_t date;
    struct tm tm;
    std::string pragma;
    bool is_date_valid;

    GeneralHeader()
    {
        auto t = std::chrono::system_clock::now();
        date = std::chrono::system_clock::to_time_t(t);
        pragma = "";
        is_date_valid = false;
    };

    GeneralHeader(std::string strdate, std::string reqpragma)
    {
        const char *status = strptime(strdate.c_str(), "%a, %d %b %Y %H:%M:%S GMT", &tm);

        if (status == NULL)
        {
            status = strptime(strdate.c_str(), "%a, %d-%b-%y %H:%M:%S GMT", &tm);

            if (status == NULL)
            {
                status = strptime(strdate.c_str(), "%a %b %d %H:%M:%S %Y", &tm);

                if (status == NULL)
                {
                    std::cerr << "Cannot process time" << std::endl;
                    return;
                }
            }
        }

        date = mktime(&tm);

        pragma = reqpragma;

        is_date_valid = true;
    }

    int setValue(std::string field, std::string value)
    {

        if (field == "Date")
        {
            if (is_date_valid)
            {
                return -1;
            }

            const char *status = strptime(value.c_str(), "%a, %d %b %Y %H:%M:%S GMT", &tm);

            if (status == NULL)
            {
                status = strptime(value.c_str(), "%a, %d-%b-%y %H:%M:%S GMT", &tm);

                if (status == NULL)
                {
                    status = strptime(value.c_str(), "%a %b %d %H:%M:%S %Y", &tm);

                    if (status == NULL)
                    {
                        std::cerr << "Cannot process time" << std::endl;
                        return -1;
                    }
                }
            }

            date = mktime(&tm);
            is_date_valid = true;

            return 0;
        }
        else if (field == "Pragma" && pragma.empty())
        {
            pragma = value;
            return 0;
        }

        return -1;
    }
};