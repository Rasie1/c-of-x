#pragma once
#include "Expressions/Function.h"

class Quote : public Function
{
public:
    Quote() {}

    ExpPtr apply(ExpPtrArg e, Environment& env) const override;
    virtual std::string show() const;
    static const std::string defaultName;
};

class QuotedExpression : public Data
{
public:
    QuotedExpression(ExpPtrArg e) : e(e) {}

    virtual std::string show() const;
    static const std::string defaultName;

    bool unapplyVariables(ExpPtrArg e, Environment& env) const override;

    ExpPtr e;
};
