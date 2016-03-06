#pragma once
#include "Operator.h"

class Then : public Operator
{
public:
    Then();
    ExpPtr operate(ExpPtrArg first,
                   ExpPtrArg second,
                   Environment& env) const override;

    std::string show() const override;

    static const std::string defaultName;
};
