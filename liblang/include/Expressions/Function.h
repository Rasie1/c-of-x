#pragma once
#include "Expressions/DataType.h"

class Function : public Data
{
public:
    virtual ExpPtr apply(ExpPtrArg e, Environment& env) const = 0;
    virtual ExpPtr intersect(ExpPtrArg other, const Environment& env) override;
    virtual ExpPtr codomain();
    virtual ExpPtr domain();
};

class ReversibleFunction : public Function
{
public:
    virtual std::shared_ptr<Function> reverse() const = 0;
    bool unapplyVariables(ExpPtrArg e, ExpPtrArg arg, Environment& env) const;
};
