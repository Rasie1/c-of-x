#pragma once
#include "Expressions/Error.h"

class ParseError : public ErrorWithMessage
{
public:
    ParseError(const std::string& msg) : ErrorWithMessage(msg) {}

    std::string show() const override;
};
