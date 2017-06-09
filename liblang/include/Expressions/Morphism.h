#pragma once
#include "Expressions/Data.h"

class Morphism : public Expression
{
public:
    virtual Object intersect(const Object& other, Environment& env) override;
    virtual Object codomain();
    virtual Object domain();
    virtual Object inverse();

    virtual Object eval(Environment& env) override;
    virtual bool unapplyVariables(const Object& e, Environment& env) override;

};

class Isomorphism : public Morphism
{
public:
    bool unapplyVariables(const Object& e, Environment& env) override;
};

