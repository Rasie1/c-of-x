#pragma once
#include "Expressions/Expression.h"

class Data : public Expression
{
public:
    Object eval(Environment& env) override;
    //virtual bool operator==(const Expression& other) const = 0;

    virtual bool unapplyVariables(const Object& e, Environment& env) override;
private:
};

class PlainData : public Data
{
public:
    virtual Object intersect(const Object& other, Environment& env) override;
};
