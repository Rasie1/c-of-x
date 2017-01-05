#pragma once
#include "Expressions/Expression.h"

class Data : public Expression
{
public:
    ExpPtr eval(Environment& env) const override;
    //virtual bool operator==(const Expression& other) const = 0;

    virtual bool unapplyVariables(ExpPtrArg e, Environment& env) const override;
private:
};

class PlainData : public Data
{
public:
    virtual ExpPtr intersect(ExpPtrArg other, const Environment& env) override;
};
