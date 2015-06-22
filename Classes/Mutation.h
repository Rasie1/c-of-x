#pragma once
#include "Operator.h"

namespace Language
{

class Mutation : public Operator
{
public:
    ExpPtr operate(const ExpPtr&  first,
                   const ExpPtr&  second,
                   Environment*& env) const override;

    std::string toString() const override;

    static const std::string defaultName;
private:
};

}
