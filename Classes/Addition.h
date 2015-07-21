#pragma once
#include "Operator.h"

namespace Language
{

class Addition : public Operator
{
public:
    Addition();
    ExpPtr operate(const ExpPtr& first,
                   const ExpPtr& second,
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
