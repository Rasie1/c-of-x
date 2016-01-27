#pragma once
#include "Function.h"
#include "Operator.h"

class Pair : public Operator
{
public:
    Pair();

    ExpPtr operate(ExpPtrArg first,
                   ExpPtrArg second,
                   Environment*& env) const override;
    std::string show() const override;

    static const std::string defaultName;
};

