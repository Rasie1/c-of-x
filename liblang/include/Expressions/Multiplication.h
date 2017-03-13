#pragma once
#include "Expressions/CalculationOperator.h"

class Multiplication : public CalculationOperator
{
public:
    Multiplication();

    ExpPtr calculate(ExpPtrArg l, ExpPtrArg r) const override;
    bool unapplyVariables(ExpPtrArg e, ExpPtrArg l, ExpPtrArg r, Environment &env) override;

    std::string show() const override;
    static const std::string defaultName;
};
