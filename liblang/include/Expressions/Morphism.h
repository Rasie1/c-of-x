#pragma once
#include "Expressions/DataType.h"

class Morphism : public Data
{
public:
    virtual ExpPtr intersect(ExpPtrArg other, const Environment& env) override;
    virtual ExpPtr codomain();
    virtual ExpPtr domain();
    virtual ExpPtr inverse() const;
};

class Isomorphism : public Morphism
{
public:
    bool unapplyVariables(ExpPtrArg e, Environment& env) override;
};

