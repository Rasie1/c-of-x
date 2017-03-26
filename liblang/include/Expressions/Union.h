#pragma once
#include "Expressions/Operator.h"
#include "Expressions/Operation.h"
#include "Expressions/Void.h"
#include <vector>

class Union : public Operator
{
public:
    Union();

    Object operate(const Object& first,
                   const Object& second,
                   Environment& env) override;

    std::string show() const override;


    Object intersect(const Object& l, const Object& r, const Object& e, Environment &env) override;
    bool unapplyVariables(const Object& e, const Object& l, const Object& r, Environment &env) override;

    static Object make(const std::vector<Object>::iterator& begin, 
                       const std::vector<Object>::iterator& end,
                       Environment& make);

    static const std::string defaultName;
};
