#pragma once
#include "Expression.h"

namespace Language
{

class EvalDelay : public Expression
{
public:
    EvalDelay() {}

    ExpPtr apply(ExpPtrArg e, Environment*& env) const override;
    virtual std::string toString() const;
    static const std::string defaultName;
};

}
