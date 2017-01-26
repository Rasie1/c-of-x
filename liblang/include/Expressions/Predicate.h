#pragma once
#include "Expressions/Function.h"

class Predicate :
        public ReversibleFunction
{
public:
    Predicate() {}

    ExpPtr apply(ExpPtrArg e, Environment& env) const override;
    ExpPtr reverse() const override;

    virtual bool holds(ExpPtrArg e, const Environment& env) const = 0;
};

