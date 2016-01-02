#pragma once
#include "Expression.h"

class EvalForce : public Expression
{
public:
    EvalForce() {}

    ExpPtr apply(ExpPtrArg e, Environment*& env) const override;
    virtual std::string show() const;
    static const std::string defaultName;
};

