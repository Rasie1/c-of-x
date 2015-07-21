#include "Assignment.h"
#include "Variable.h"
#include "Environment.h"
#include "Pattern.h"
#include "Operation.h"
#include "Lambda.h"

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

    ExpPtr rvalue = second->eval(env);
    ExpPtr lvalue = first;
    while (lvalue->unwind(lvalue, rvalue, env));
    env = env->add(lvalue, rvalue->eval(env));

    return second;
}

std::string Assignment::toString() const
{
    return Assignment::defaultName;
}

const std::string Assignment::defaultName = "=";

}
