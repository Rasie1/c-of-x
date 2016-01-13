#pragma once
#include "CalculationOperator.h"

class Addition : public CalculationOperator
{
public:
    Addition();

    ExpPtr operate(ExpPtrArg first,
                   ExpPtrArg second,
                   Environment*& env) const override;

    std::string show() const override;

    virtual bool unwind(ExpPtr& left,
                        ExpPtr& right,
                        ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment*& env) override;

    static const std::string defaultName;
};
