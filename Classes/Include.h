#pragma once
#include "Expression.h"

namespace Language
{

class Include : public Expression
{
public:
    ExpPtr apply(ExpPtrArg e, Environment*& env) const override;
    std::string show() const override;
    static const std::string defaultName;
};

}
