#pragma once
#include "Expressions/Data.h"
#include <string>

class Error : public Data
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
