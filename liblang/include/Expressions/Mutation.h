#pragma once
#include "Expressions/Operator.h"

class Mutation : public Operator
{
public:
    Object operate(const Object&  first,
                   const Object&  second,
                   Environment& env) override;

    std::string show() const override;

    static const std::string defaultName;
private:
};

