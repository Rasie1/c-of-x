#pragma once
#include "Expressions/Morphism.h"

class Quote : public Morphism
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
