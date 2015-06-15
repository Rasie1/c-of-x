#include "EvalForce.h"
#include "TypeError.h"

namespace Language
{

Expression* EvalForce::apply(Expression* e, Environment*& env)
{
    return e->evalConstEnv(env)->evalConstEnv(env);
}

std::string EvalForce::toString() const
{
    return defaultName;
}

const std::string EvalForce::defaultName = "#";


}
