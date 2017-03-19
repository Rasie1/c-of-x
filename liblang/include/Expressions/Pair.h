#pragma once
#include "Expressions/Morphism.h"
#include "Expressions/Operator.h"

class Pair : public Operator
{
public:
    Pair();

    Object operate(const Object& first,
                   const Object& second,
                   Environment& env) override;
    std::string show() const override;

    static const std::string defaultName;
};

