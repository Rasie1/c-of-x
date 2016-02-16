#pragma once
#include "Function.h"

class Predicate : public Function
{
public:
    Predicate() {}

    ExpPtr apply(ExpPtrArg e, Environment*& env) const override;

    virtual bool holds(ExpPtrArg e, Environment* env) const = 0;
};

class Equals : public Predicate
{
public:
    Equals(ExpPtrArg e) : value(e) {}
    bool holds(ExpPtrArg e, Environment* env) const override;

    ExpPtr value;

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
