#pragma once
#include "Function.h"
#include "Integer.h"
#include "Operator.h"

namespace Language
{

class Subtraction : public Operator
{
public:
    Subtraction();

    ExpPtr operate(const ExpPtr&  first,
                   const ExpPtr&  second,
                   Environment*& env) const override;

    std::string toString() const override;

    virtual bool unwind(ExpPtr& left,
                        ExpPtr& right,
                        ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment*& env) override;

    static const std::string defaultName;
};

}
