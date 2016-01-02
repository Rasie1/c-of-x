#pragma once
#include "Function.h"
#include "Integer.h"
#include "Operator.h"

class Multiplication : public Operator
{
public:
    Multiplication();

    ExpPtr operate(ExpPtrArg  first,
                   ExpPtrArg  second,
                   Environment*& env) const override;

    std::string show() const override;

    static const std::string defaultName;
};

