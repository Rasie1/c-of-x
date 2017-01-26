#pragma once
#include "Expressions/Morphism.h"
#include "Expressions/Operator.h"

class Pair : public Operator
{
public:
    Pair();

    ExpPtr operate(ExpPtrArg first,
                   ExpPtrArg second,
                   Environment& env) const override;
    std::string show() const override;

    static const std::string defaultName;
};

