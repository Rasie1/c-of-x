#pragma once
#include "Operator.h"

class DefaultOperator : public Operator
{
public:
    DefaultOperator();

    ExpPtr operate(ExpPtrArg first,
                   ExpPtrArg second,
                   Environment& env) const override;

    std::string show() const override;

    static const std::string defaultName;
};
