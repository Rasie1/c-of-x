#pragma once
#include "Expressions/Predicate.h"

class ValueInSet : public Data
{
public:
    ValueInSet(ExpPtrArg set) : set(set) {}

    ExpPtr eval(Environment& env) const override;

    ExpPtr intersect(ExpPtrArg other, const Environment& env) override;
    bool unapplyVariables(ExpPtrArg e, Environment& env) const override;

    virtual std::string show() const;

    ExpPtr set;
};
