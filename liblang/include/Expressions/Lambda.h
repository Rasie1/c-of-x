#pragma once
#include "Expressions/Expression.h"
#include "Expressions/Operator.h"

class Lambda : public Operator
{
public:
    Lambda();

    ExpPtr operate(ExpPtrArg first,
                   ExpPtrArg second,
                   Environment& env) const override;

    std::string show() const override;

    static const std::string defaultName;
private:
};
