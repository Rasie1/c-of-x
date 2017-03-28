#pragma once
#include "Expressions/Type.h"

class IntegerType : public Type
{
public:
    optional<bool> holds(const Object& e, Environment& env) override;

    std::string show() const;
    static const std::string defaultName;
};
