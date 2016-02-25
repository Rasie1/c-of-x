#pragma once
#include "CalculationOperator.h"

class Subtraction : public CalculationOperator
{
public:
    Subtraction();

    ExpPtr calculate(ExpPtrArg l, ExpPtrArg r) const override;

    std::string show() const override;

    virtual bool unwind(ExpPtr& left,
                        ExpPtr& right,
                        ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment& env) override;

    static const std::string defaultName;
};
