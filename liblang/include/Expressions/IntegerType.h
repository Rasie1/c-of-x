#pragma once
#include "Expressions/Type.h"

class IntegerType : public Type
{
public:
    optional<bool> holds(ExpPtrArg e, const Environment& env) const override;

    std::string show() const;
    static const std::string defaultName;
};
