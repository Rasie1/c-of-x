#pragma once
#include "Expressions/CalculationOperator.h"

class Subtraction : public CalculationOperator
{
public:
    Subtraction();

    Object calculate(const Object& l, const Object& r) const override;
    bool unapplyVariables(const Object& e, const Object& l, const Object& r, Environment &env) override;

    std::string show() const override;
    static const std::string defaultName;
};
