#include "Pair.h"
#include "Operation.h"

namespace Language
{

ExpPtr Pair::operate(const ExpPtr& first,
                     const ExpPtr& second,
                     Environment*& env) const
{
    Environment* newEnv = env;
    first->eval(newEnv);
    return second->eval(newEnv);
}

std::string Pair::toString() const
{
    return Pair::defaultName;
}

const std::string Pair::defaultName = ";";


}
