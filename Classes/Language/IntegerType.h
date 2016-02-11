#pragma once
#include "Function.h"

class IntegerType : public Function
{
public:
    ExpPtr apply(ExpPtrArg e, Environment*& env) const override;

    std::string show() const;
    static const std::string defaultName;
};
