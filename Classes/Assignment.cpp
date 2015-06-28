#include "Assignment.h"
#include "Variable.h"
#include "Environment.h"
#include "Pattern.h"
#include "Operation.h"

namespace Language
{

Assignment::Assignment()
    : Operator(true, 2)
{

}

ExpPtr Assignment::operate(const ExpPtr& first,
                           const ExpPtr& second,
                           Environment*& env) const
{
    auto newEnv = env;
    env = env->add(first->pattern(),
                   second->eval(newEnv));
    newEnv = env;
    return second->eval(newEnv);
}

std::string Assignment::toString() const
{
    return Assignment::defaultName;
}

const std::string Assignment::defaultName = "=";

}
