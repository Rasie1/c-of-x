#pragma once
#include "Function.h"
#include "Integer.h"
#include "Operator.h"

namespace Language
{

class Addition : public Operator
{
public:
    Expression* operate(Expression* first,
                        Expression* second,
                        Environment*& env) override;
    std::string toString() const override;

    static const std::string defaultName;
};

}
