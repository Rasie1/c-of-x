#pragma once
#include "Expressions/Function.h"

class EvalForce : public Function
{
public:
    EvalForce() {}

    ExpPtr apply(ExpPtrArg e, Environment& env) const override;
    virtual std::string show() const;
    static const std::string defaultName;
};

