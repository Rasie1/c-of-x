#pragma once
#include "Function.h"

class EvalDelay : public Function
{
public:
    EvalDelay() {}

    ExpPtr apply(ExpPtrArg e, Environment& env) const override;
    virtual std::string show() const;
    static const std::string defaultName;
};
