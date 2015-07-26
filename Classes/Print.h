#pragma once
#include "Expression.h"

namespace Language
{

class Print : public Expression
{
public:
    Print() {}

    ExpPtr apply(ExpPtrArg e,
                 Environment*& env) const override;
    virtual std::string toString() const;
    static const std::string defaultName;
};

class Show : public Expression
{
public:
    Show() {}

    ExpPtr apply(ExpPtrArg e,
                 Environment*& env) const override;
    virtual std::string toString() const;
    static const std::string defaultName;
};

class PrintPattern : public Expression
{
public:
    ExpPtr apply(ExpPtrArg e,
                 Environment*& env) const override;
    std::string toString() const override;
    static const std::string defaultName;
};


}
