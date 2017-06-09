#pragma once
#include "Expressions/Morphism.h"

class ValueInSet : public Morphism
{
public:
    ValueInSet(const Object& set) : set(set) {}

    Object eval(Environment& env) override;

    Object intersect(const Object& other, Environment& env) override;
    bool unapplyVariables(const Object& e, Environment& env) override;

    virtual std::string show() const;

    Object set;
};
