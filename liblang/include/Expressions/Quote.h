#pragma once
#include "Expressions/Morphism.h"

class Quote : public Morphism
{
public:
    Quote() {}

    Object apply(const Object& e, Environment& env) override;
    virtual std::string show() const;
    static const std::string defaultName;
};

class QuotedExpression : public Data
{
public:
    QuotedExpression(const Object& e) : e(e) {}

    virtual std::string show() const;
    static const std::string defaultName;

    bool unapplyVariables(const Object& e, Environment& env) override;

    Object e;
};
