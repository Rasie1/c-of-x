#pragma once
#include "Function.h"

class Pattern : public Expression
{
public:
    Pattern();

    virtual bool match(ExpPtrArg other) const override;
    virtual std::string show() const override;
    PatPtr pattern() const override;
};

class ReturnPattern : public Function
{
public:
    ExpPtr apply(ExpPtrArg e,
                 Environment*& env) const override;
    std::string show() const override;
    static const std::string defaultName;
};

