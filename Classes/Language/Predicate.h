#pragma once
#include "Function.h"

class Predicate : public Function
{
public:
    Predicate() {}

    ExpPtr apply(ExpPtrArg e, Environment*& env) const override;

    virtual bool holds(ExpPtrArg e, Environment*& env) const = 0;
};
