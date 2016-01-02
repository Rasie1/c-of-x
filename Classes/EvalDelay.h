#pragma once
#include "Expression.h"

class EvalDelay : public Expression
{
public:
    EvalDelay() {}

    ExpPtr apply(ExpPtrArg e, Environment*& env) const override;
    virtual std::string show() const;
    static const std::string defaultName;
};
