#include <string>

#ifndef METHOD
#define METHOD
enum Method
{
    GET,
    HEAD,
    POST,
    ERROR
};


extern Method get_method(std::string &token)
{
    if (token == "GET")
    {
        return GET;
    }
    else if (token == "HEAD")
    {
        return HEAD;
    }
    else if (token == "POST")
    {
        return POST;
    }
    else
    {
        return ERROR;
    }
}

#endif
