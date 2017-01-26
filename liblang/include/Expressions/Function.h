#pragma once
#include "Expressions/DataType.h"

class Function : public Data
{
public:
    virtual ExpPtr apply(ExpPtrArg e, Environment& env) const = 0;
    virtual ExpPtr intersect(ExpPtrArg other, const Environment& env) override;
    virtual ExpPtr codomain();
    virtual ExpPtr domain();
    virtual bool   reversible() const;
    virtual ExpPtr reverse() const;
};

class ReversibleFunction : public Function
{
public:
    bool reversible() const override;
    bool unapplyVariables(ExpPtrArg e, Environment& env) const override;
};

