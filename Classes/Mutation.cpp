#include "Mutation.h"
#include "Variable.h"
#include "Environment.h"
#include "Pattern.h"
#include <string>
#include "Operation.h"

namespace Language
{

Expression* Mutation::operate(Expression* first,
                              Expression* second,
                              Environment*& env)
{
    throw std::logic_error("Mutate is still in TODO state");
    env = env->add(first->pattern(),
                   second->evalConstEnv(env));
    return second;
}

std::string Mutation::toString() const
{
    return Mutation::defaultName;
}

const std::string Mutation::defaultName = ":=";

}
