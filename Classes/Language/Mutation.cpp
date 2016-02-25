#include "Mutation.h"
#include "Identifier.h"
#include "Environment.h"
#include <string>
#include "Operation.h"


ExpPtr Mutation::operate(ExpPtrArg first,
                               ExpPtrArg second,
                               Environment& env) const
{
    throw std::logic_error("Mutate is still in TODO state");
    return second;
}

std::string Mutation::show() const
{
    return Mutation::defaultName;
}

const std::string Mutation::defaultName = "<-";
