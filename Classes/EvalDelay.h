#pragma once
#include "Expression.h"

namespace Language
{

class EvalDelay : public Expression
{
public:
    EvalDelay() {}

    Expression* apply(Expression* e, Environment* env) override;
    virtual std::string toString() const;
    static const std::string defaultName;
};

}
