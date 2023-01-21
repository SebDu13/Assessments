#pragma once

#include <iostream>
#include <optional>

namespace helper 
{
int guard(int n, const char * err);
const char* getCmdOption(const char ** begin, const char ** end, const std::string & option);

std::optional<uint16_t> convertPort(const char* port);
std::optional<std::string> convertIpAddress(const char* ipAddress);
}