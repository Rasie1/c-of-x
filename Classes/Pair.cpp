#include "Pair.h"
#include "Operation.h"

Pair::Pair()
    : Operator(true, 0)
{

}

ExpPtr Pair::operate(ExpPtrArg first,
                     ExpPtrArg second,
                     Environment*& env) const
{
    first->eval(env);
    return second->eval(env);
}

std::string Pair::show() const
{
    return Pair::defaultName;
}

const std::string Pair::defaultName = ",";

