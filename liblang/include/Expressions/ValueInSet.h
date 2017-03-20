#pragma once
#include "Expressions/DataType.h"

class ValueInSet : public Data
{
public:
    ValueInSet(const Object& set) : set(set) {}

    Object eval(Environment& env) override;

    Object intersect(const Object& other, const Environment& env) override;
    bool unapplyVariables(const Object& e, Environment& env) override;

    virtual std::string show() const;

    Object set;
};
