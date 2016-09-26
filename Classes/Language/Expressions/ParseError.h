#pragma once
#include "Error.h"

class ParseError : public ErrorWithMessage
{
public:
    ParseError(const std::string& msg) : ErrorWithMessage(msg) {}
};
