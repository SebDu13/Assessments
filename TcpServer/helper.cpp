#include "helper.hpp"

namespace helper 
{

int guard(int n, const char * err) 
{ 
    if (n == -1) 
    {
        throw(std::runtime_error(err));
    }
    return n; 
}

const char* getCmdOption(const char ** begin, const char ** end, const std::string & option)
{
    const char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return nullptr;
}

std::optional<uint16_t> convertPort(const char* port)
{
    if(!port)
        return std::nullopt;
    return atoi(port);
}

std::optional<std::string> convertIpAddress(const char* ipAddress)
{
    if(!ipAddress)
        return std::nullopt;
    return std::string(ipAddress);
}

}