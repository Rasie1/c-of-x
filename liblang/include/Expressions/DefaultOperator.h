#pragma once
#include "Expressions/Operator.h"

class DefaultOperator : public Operator
{
public:
    DefaultOperator();

    Object operate(const Object& first,
                   const Object& second,
                   Environment& env) override;

    std::string show() const override;

    static const std::string defaultName;
};
