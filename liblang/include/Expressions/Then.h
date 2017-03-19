#pragma once
#include "Expressions/Operator.h"

class Then : public Operator
{
public:
    Then();
    Object operate(const Object& first,
                   const Object& second,
                   Environment& env) override;

    std::string show() const override;

    static const std::string defaultName;
};
