#include "Expressions/EvalForce.h"
#include "Expressions/TypeError.h"

ExpPtr EvalForce::apply(ExpPtrArg e, Environment& env)
{
    return e->eval(env)->eval(env);
}

std::string EvalForce::show() const
{
    return defaultName;
}

const std::string EvalForce::defaultName = "#";

