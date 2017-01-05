#include "Expressions/Mutation.h"
#include "Expressions/Identifier.h"
#include "System/Environment.h"
#include "Expressions/Operation.h"
#include "Expressions/Equality.h"


ExpPtr Mutation::operate(ExpPtrArg first,
                         ExpPtrArg second,
                         Environment& env) const
{
    // still in todo state (can't use the same variable)
    env.erase(first);
    Equality().operate(first, second, env);

    return second;
}

std::string Mutation::show() const
{
    return Mutation::defaultName;
}

const std::string Mutation::defaultName = "<-";
