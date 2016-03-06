#pragma once
#include "CalculationOperator.h"

class Subtraction : public CalculationOperator
{
public:
    Subtraction();

    ExpPtr calculate(ExpPtrArg l, ExpPtrArg r) const override;

    std::string show() const override;

    static const std::string defaultName;
};
