#include "Mutation.h"
#include "Variable.h"
#include "Environment.h"
#include "Pattern.h"
#include <string>
#include "Operation.h"

namespace Language
{

ExpPtr Mutation::operate(ExpPtrArg first,
                               ExpPtrArg second,
                               Environment*& env) const
{
    throw std::logic_error("Mutate is still in TODO state");
    auto newEnv = env;
    env = env->add(first->pattern(),
                   second->eval(newEnv));
    return second;
}

std::string Mutation::show() const
{
    return Mutation::defaultName;
}

const std::string Mutation::defaultName = ":=";

}
