#pragma once
#include "Function.h"
#include "Integer.h"
#include "Operator.h"

namespace Language
{

class Multiplication : public Operator
{
public:
    Multiplication();

    ExpPtr operate(ExpPtrArg  first,
                   ExpPtrArg  second,
                   Environment*& env) const override;

    std::string toString() const override;

    static const std::string defaultName;
};

}
