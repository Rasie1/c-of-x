#pragma once
#include "Function.h"
#include "Integer.h"
#include "Operator.h"

namespace Language
{

class Multiplication : public Operator
{
public:
    ExpPtr operate(const ExpPtr&  first,
                   const ExpPtr&  second,
                   Environment*& env) const override;

    std::string toString() const override;

    static const std::string defaultName;
};

}
