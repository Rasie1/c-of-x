#include "Assignment.h"
#include "Variable.h"
#include "Environment.h"
#include "Operation.h"
#include "Lambda.h"
#include "Closure.h"

Assignment::Assignment()
    : Operator(true, 2)
{

}

ExpPtr Assignment::operate(ExpPtrArg first,
                           ExpPtrArg second,
                           Environment*& env) const
{
    ExpPtr rvalue = second;
    ExpPtr lvalue = first;
    while (lvalue->unwind(lvalue, rvalue, env));
    env = env->add(lvalue, rvalue->eval(env));

    // Enable recursion
    if (d_cast<Function>(rvalue))
        env = env->add(lvalue, rvalue);

    return second;
}

std::string Assignment::show() const
{
    return Assignment::defaultName;
}

const std::string Assignment::defaultName = "=";
