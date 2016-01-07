#pragma once
#include "Function.h"

class Print : public Function
{
public:
    Print() {}

    ExpPtr apply(ExpPtrArg e,
                 Environment*& env) const override;
    virtual std::string show() const;
    static const std::string defaultName;
};

class Show : public Function
{
public:
    Show() {}

    ExpPtr apply(ExpPtrArg e,
                 Environment*& env) const override;
    virtual std::string show() const;
    static const std::string defaultName;
};

class PrintPattern : public Function
{
public:
    ExpPtr apply(ExpPtrArg e,
                 Environment*& env) const override;
    std::string show() const override;
    static const std::string defaultName;
};
