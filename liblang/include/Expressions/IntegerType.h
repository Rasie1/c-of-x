#pragma once
#include "Expressions/Type.h"

class IntegerType : public Type
{
public:
    bool holds(ExpPtrArg e, const Environment& env) const override;

    std::string show() const;
    static const std::string defaultName;
};
