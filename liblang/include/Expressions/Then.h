#pragma once
#include "Expressions/Operator.h"

class Then : public Operator
{
public:
    Then();
    ExpPtr operate(ExpPtrArg first,
                   ExpPtrArg second,
                   Environment& env) override;

    std::string show() const override;

    static const std::string defaultName;
};
