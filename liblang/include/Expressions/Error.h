#pragma once
#include "Expressions/Void.h"
#include <string>

class Error : public Void
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
