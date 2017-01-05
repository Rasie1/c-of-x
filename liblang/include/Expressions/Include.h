#pragma once
#include "Expressions/Function.h"

class Include : public Function
{
public:
    ExpPtr apply(ExpPtrArg e, Environment& env) const override;
    std::string show() const override;
    static const std::string defaultName;
};

