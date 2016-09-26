#pragma once
#include "CalculationOperator.h"

class Multiplication : public CalculationOperator
{
public:
    Multiplication();

    ExpPtr calculate(ExpPtrArg l, ExpPtrArg r) const override;
    bool unapplyVariables(ExpPtrArg e, ExpPtrArg l, ExpPtrArg r, Environment &env) const override;

    std::string show() const override;
    static const std::string defaultName;
};
