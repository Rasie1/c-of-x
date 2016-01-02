#pragma once
#include "Operator.h"

class Assignment : public Operator
{
public:
    Assignment();

    ExpPtr operate(ExpPtrArg  first,
                   ExpPtrArg  second,
                   Environment*& env) const override;

    std::string show() const override;

    static const std::string defaultName;
};
