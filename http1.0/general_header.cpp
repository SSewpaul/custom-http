#include <ctime>
#include <string>
#include <chrono>

static const std::string GENERAL_HEADER_FIELDS[2] = {"Date", "Pragma"};

class GeneralHeader {
    public:
        time_t date;
        struct tm tm;
        std::string pragma;
        bool is_date_valid;

        GeneralHeader() {
            auto t = std::chrono::system_clock::now();
            date = std::chrono::system_clock::to_time_t(t);
            pragma = "";
            is_date_valid = false;
        };

        GeneralHeader(std::string strdate, std::string reqpragma) {
            strptime(strdate.c_str(), "%a, %d %b %Y %H:%M:%S GMT", &tm);
            date = mktime(&tm);

            pragma = reqpragma;

            is_date_valid = true;
        }

        int setValue(std::string field, std::string value) {

            if (field == "Date") {
                if(is_date_valid) {
                    return -1;
                }

                strptime(value.c_str(), "%a, %d %b %Y %H:%M:%S GMT", &tm);
                date = mktime(&tm);
                is_date_valid = true;

                return 0;
            }
            else if (field == "Pragma" && pragma.empty()) {
                pragma = value;
                return 0;
            }

            return -1;
        }
};