#pragma once
#include "Expression.h"

class Include : public Expression
{
public:
    ExpPtr apply(ExpPtrArg e, Environment*& env) const override;
    std::string show() const override;
    static const std::string defaultName;
};

