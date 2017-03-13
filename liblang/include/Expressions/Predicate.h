#pragma once
#include "Expressions/Morphism.h"

class Predicate :
        public Isomorphism
{
public:
    Predicate() {}

    ExpPtr apply(ExpPtrArg e, Environment& env) override;
    ExpPtr inverse() const override;

    virtual optional<bool> holds(ExpPtrArg e, const Environment& env) = 0;
};

