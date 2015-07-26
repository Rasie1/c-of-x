#pragma once
#include "Function.h"
#include "Operator.h"

namespace Language
{

class Pair : public Operator
{
public:
    Pair();

    ExpPtr operate(ExpPtrArg first,
                   ExpPtrArg second,
                   Environment*& env) const override;
    std::string toString() const override;

    static const std::string defaultName;
};

}
