#pragma once
#include "Expressions/Function.h"

class Not : public Function
{
public:
    Not() {}

    ExpPtr apply(ExpPtrArg e, Environment& env) const override;
    virtual std::string show() const;
    static const std::string defaultName;
};

class Complement : public Data
{
public:
    Complement(ExpPtrArg e) : e(e) {}

    virtual std::string show() const;
    static const std::string defaultName;

    bool unapplyVariables(ExpPtrArg e, Environment& env) const override;

    ExpPtr e;
};
