#pragma once
#include "Expressions/Operator.h"

class CalculationOperator : public Operator
{
public:
    CalculationOperator(bool isRightAssociative, int priority, bool splitting);

    virtual Object calculate(const Object& l, 
                             const Object& r,
                             Environment& env) const = 0;

    Object operate(const Object& first,
                   const Object& second,
                   Environment& env) override;
};
