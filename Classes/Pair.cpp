#include "Pair.h"
#include "Operation.h"

namespace Language
{

Pair::Pair()
    : Operator(true, 0)
{

}

ExpPtr Pair::operate(const ExpPtr& first,
                     const ExpPtr& second,
                     Environment*& env) const
{
    first->eval(env);
    return second->eval(env);
}

std::string Pair::toString() const
{
    return Pair::defaultName;
}

const std::string Pair::defaultName = ";";


}
