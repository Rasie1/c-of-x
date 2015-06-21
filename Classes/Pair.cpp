#include "Pair.h"
#include "Operation.h"

namespace Language
{

Expression* Pair::operate(Expression* first,
                          Expression* second,
                          Environment*& env)
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
