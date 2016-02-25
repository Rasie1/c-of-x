#pragma once
#include "Function.h"

class Print : public Function
{
public:
    Print() {}

    ExpPtr apply(ExpPtrArg e,
                 Environment& env) const override;
    virtual std::string show() const;
    static const std::string defaultName;
};

class PrintInfo : public Function
{
public:
    PrintInfo() {}

    ExpPtr apply(ExpPtrArg e,
                 Environment& env) const override;
    virtual std::string show() const;
    static const std::string defaultName;
};
