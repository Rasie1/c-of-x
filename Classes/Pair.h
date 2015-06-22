#pragma once
#include "Function.h"
#include "Operator.h"

namespace Language
{

class Pair : public Operator
{
public:
    ExpPtr operate(const ExpPtr& first,
                   const ExpPtr& second,
                   Environment*& env) const override;
    std::string toString() const override;

    static const std::string defaultName;
};

}
