#pragma once
#include "Expression.h"

class Function : public Expression
{
public:
    // ExpPtr evaluation(Environment*& env) const override;
    virtual ExpPtr apply(ExpPtrArg e, Environment*& env) const = 0;
    // std::string show() const override;
};
