#pragma once
#include "Function.h"
#include "Operator.h"

namespace Language
{

class Pair : public Operator
{
public:
    Expression* operate(Expression* first,
                        Expression* second,
                        Environment*& env) override;
    std::string toString() const override;

    static const std::string defaultName;
};

}
