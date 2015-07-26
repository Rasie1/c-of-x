#pragma once
#include "Operator.h"

namespace Language
{

class Assignment : public Operator
{
public:
    Assignment();

    ExpPtr operate(ExpPtrArg  first,
                   ExpPtrArg  second,
                   Environment*& env) const override;

    std::string toString() const override;

    static const std::string defaultName;
};

}
