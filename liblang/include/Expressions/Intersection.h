#pragma once
#include "Expressions/Operator.h"

class Intersection : public Operator
{
public:
    Intersection();

    Object operate(const Object& first,
                   const Object& second,
                   Environment& env) override;
    
    Object apply(const Object& l, const Object& r, const Object& e, Environment &env) override;
    Object intersect(const Object& l, const Object& r, const Object& e, Environment &env);
    bool unapplyVariables(const Object& e, const Object& l, const Object& r, Environment &env) override;
    Object element(const Object& l, const Object& r, Environment& env);

    std::string show() const override;

    static const std::string defaultName;
};
