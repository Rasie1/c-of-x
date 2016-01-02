#pragma once
#include "Expression.h"

class Print : public Expression
{
public:
    Print() {}

    ExpPtr apply(ExpPtrArg e,
                 Environment*& env) const override;
    virtual std::string show() const;
    static const std::string defaultName;
};

class Show : public Expression
{
public:
    Show() {}

    ExpPtr apply(ExpPtrArg e,
                 Environment*& env) const override;
    virtual std::string show() const;
    static const std::string defaultName;
};

class PrintPattern : public Expression
{
public:
    ExpPtr apply(ExpPtrArg e,
                 Environment*& env) const override;
    std::string show() const override;
    static const std::string defaultName;
};
