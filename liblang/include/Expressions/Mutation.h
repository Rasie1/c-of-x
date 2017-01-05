#pragma once
#include "Expressions/Operator.h"

class Mutation : public Operator
{
public:
    ExpPtr operate(ExpPtrArg  first,
                   ExpPtrArg  second,
                   Environment& env) const override;

    std::string show() const override;

    static const std::string defaultName;
private:
};

