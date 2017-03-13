#pragma once
#include "Expressions/Morphism.h"

class EvalForce : public Morphism
{
public:
    EvalForce() {}

    ExpPtr apply(ExpPtrArg e, Environment& env) override;
    virtual std::string show() const;
    static const std::string defaultName;
};

