#pragma once
#include "Expression.h"

namespace Language
{

class EvalForce : public Expression
{
public:
    EvalForce() {}

    Expression* apply(Expression* e, Environment*& env) override;
    virtual std::string toString() const;
    static const std::string defaultName;
};

}
