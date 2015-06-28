#pragma once
#include "Operator.h"

namespace Language
{

class Assignment : public Operator
{
public:
    Assignment();

    ExpPtr operate(const ExpPtr&  first,
                   const ExpPtr&  second,
                   Environment*& env) const override;

    std::string toString() const override;

    static const std::string defaultName;
};

}
