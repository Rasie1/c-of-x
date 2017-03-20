#pragma once
#include "Expressions/DataType.h"

class Morphism : public Data
{
public:
    virtual Object intersect(const Object& other, const Environment& env) override;
    virtual Object codomain();
    virtual Object domain();
    virtual Object inverse();
};

class Isomorphism : public Morphism
{
public:
    bool unapplyVariables(const Object& e, Environment& env) override;
};

