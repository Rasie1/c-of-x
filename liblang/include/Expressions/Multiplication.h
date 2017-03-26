#pragma once
#include "Expressions/CalculationOperator.h"

class Multiplication : public CalculationOperator
{
public:
    Multiplication();

    Object calculate(const Object& l, 
                     const Object& r,
                     Environment& env) const override;
    bool unapplyVariables(const Object& e, const Object& l, const Object& r, Environment &env) override;

    std::string show() const override;
    static const std::string defaultName;
};
