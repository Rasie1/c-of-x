#pragma once
#include <exception>
#include <string>

struct ExceptionParseError : public std::exception
{
    ExceptionParseError(const std::string& s)
        : s(s) {}

    std::string s;
};

