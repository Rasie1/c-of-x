#pragma once
#include "Predicate.h"

class Equals : public Predicate
{
public:
    Equals(ExpPtrArg e) : value(e) {}
    bool holds(ExpPtrArg e, Environment* env) const override;

    ExpPtr value;

    ExpPtr intersect(ExpPtrArg other, Environment* env) override;

    std::string show() const;
    static const std::string defaultName;
};

class Equality : public Function
{
public:
    ExpPtr apply(ExpPtrArg e, Environment*& env) const override;

    std::string show() const;
    static const std::string defaultName;
};
