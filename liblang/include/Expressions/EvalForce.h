#pragma once
#include "Expressions/Morphism.h"

class EvalForce : public Morphism
{
public:
    EvalForce() {}

    Object apply(const Object& e, Environment& env) override;
    virtual std::string show() const;
    static const std::string defaultName;
};

