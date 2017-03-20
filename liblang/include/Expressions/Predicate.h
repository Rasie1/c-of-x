#pragma once
#include "Expressions/Morphism.h"

class Predicate :
        public Isomorphism
{
public:
    Predicate() {}

    Object apply(const Object& e, Environment& env) override;
    Object inverse() override;

    virtual optional<bool> holds(const Object& e, const Environment& env) = 0;
};

