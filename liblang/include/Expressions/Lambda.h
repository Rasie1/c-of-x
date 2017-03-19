#pragma once
#include "Expressions/Expression.h"
#include "Expressions/Operator.h"

class Lambda : public Operator
{
public:
    Lambda();

    Object operate(const Object& first,
                   const Object& second,
                   Environment& env) override;

    std::string show() const override;
    // bool unapplyVariables(const Object& e, 
    //                       const Object& l, const Object& r, 
    //                       Environment& env) const override;

    static const std::string defaultName;
private:
};
