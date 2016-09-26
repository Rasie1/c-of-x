#pragma once
#include "Function.h"

class Predicate :
        public ReversibleFunction
{
public:
    Predicate() {}

    ExpPtr apply(ExpPtrArg e, Environment& env) const override;
    std::shared_ptr<Function> reverse() const override;

    virtual bool holds(ExpPtrArg e, const Environment& env) const = 0;
};