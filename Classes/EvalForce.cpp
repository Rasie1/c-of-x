#include "EvalForce.h"
#include "TypeError.h"

namespace Language
{

ExpPtr EvalForce::apply(const ExpPtr& e, Environment*& env) const
{
    return e->evalConstEnv(env)->evalConstEnv(env);
}

std::string EvalForce::toString() const
{
    return defaultName;
}

const std::string EvalForce::defaultName = "#";


}
