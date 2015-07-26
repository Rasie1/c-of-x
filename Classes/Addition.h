#pragma once
#include "Operator.h"

namespace Language
{

class Addition : public Operator
{
public:
    Addition();
    ExpPtr operate(ExpPtrArg first,
                   ExpPtrArg second,
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
