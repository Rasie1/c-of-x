#pragma once
#include "Operator.h"

class CalculationOperator : public Operator
{
public:
    CalculationOperator(bool isRightAssociative, int priority);

//    ExpPtr operate(ExpPtrArg first,
//                   ExpPtrArg second,
//                   Environment*& env) const override;

//    std::string show() const override;

//    virtual bool unwind(ExpPtr& left,
//                        ExpPtr& right,
//                        ExpPtr& lvalue,
//                        ExpPtr& rvalue,
//                        Environment*& env) override;
};
