#include "EvalForce.h"
#include "TypeError.h"

namespace Language
{

ExpPtr EvalForce::apply(ExpPtrArg e, Environment*& env) const
{
    return e->eval(env)->eval(env);
}

std::string EvalForce::toString() const
{
    return defaultName;
}

const std::string EvalForce::defaultName = "#";


}
