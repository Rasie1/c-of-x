#include "Expressions/EvalForce.h"
#include "Expressions/TypeError.h"

Object EvalForce::apply(const Object& e, Environment& env)
{
    return e;
}

std::string EvalForce::show() const
{
    return defaultName;
}

const std::string EvalForce::defaultName = "#";

