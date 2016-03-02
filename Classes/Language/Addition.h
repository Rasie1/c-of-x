#pragma once
#include "CalculationOperator.h"

class Addition : public CalculationOperator
{
public:
    Addition();

    ExpPtr calculate(ExpPtrArg l, ExpPtrArg r) const override;

    std::string show() const override;

    void unapplyVariables(ExpPtrArg e, ExpPtrArg l, ExpPtrArg r, Environment &env) const override;

    virtual bool unwind(ExpPtr& left,
                        ExpPtr& right,
                        ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment& env) override;

    static const std::string defaultName;
};
