#include "EvalForce.h"
#include "TypeError.h"

ExpPtr EvalForce::apply(ExpPtrArg e, Environment*& env) const
{
    return e->eval(env)->eval(env);
}

std::string EvalForce::show() const
{
    return defaultName;
}

const std::string EvalForce::defaultName = "#";

