#include "Assignment.h"
#include "Variable.h"
#include "Environment.h"
#include "Pattern.h"
#include "Operation.h"

namespace Language
{

ExpPtr Assignment::operate(const ExpPtr& first,
                         const ExpPtr& second,
                         Environment*& env) const
{
    env = env->add(first->pattern(),
                   second->evalConstEnv(env));
    return second->evalConstEnv(env);
}

std::string Assignment::toString() const
{
    return Assignment::defaultName;
}

const std::string Assignment::defaultName = "=";

}
