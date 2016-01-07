#pragma once
#include "Expression.h"
#include <string>

class Error : public Expression
{
public:
    Error() {}

    virtual std::string show() const override;
};

class ErrorWithMessage : public Error
{
public:
    ErrorWithMessage(const std::string& msg) : message(msg) {}

    std::string show() const override;
private:
    std::string message;
};
