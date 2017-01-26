#pragma once
#include "Expressions/DataType.h"

class Morphism : public Data
{
public:
    virtual ExpPtr apply(ExpPtrArg e, Environment& env) const = 0;
    virtual ExpPtr intersect(ExpPtrArg other, const Environment& env) override;
    virtual ExpPtr codomain();
    virtual ExpPtr domain();
    virtual bool   inversible() const;
    virtual ExpPtr inverse() const;
};

class Isomorphism : public Morphism
{
public:
    bool inversible() const override;
    bool unapplyVariables(ExpPtrArg e, Environment& env) const override;
};

