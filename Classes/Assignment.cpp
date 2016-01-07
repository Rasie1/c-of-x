#include "Assignment.h"
#include "Variable.h"
#include "Environment.h"
#include "Operation.h"
#include "Lambda.h"

Assignment::Assignment()
    : Operator(true, 2)
{

}

ExpPtr Assignment::operate(ExpPtrArg first,
                           ExpPtrArg second,
                           Environment*& env) const
{

    ExpPtr rvalue = second;//->eval(env);
    ExpPtr lvalue = first;
    while (lvalue->unwind(lvalue, rvalue, env));
    env = env->add(lvalue, rvalue->eval(env));

    return second;
}

std::string Assignment::show() const
{
    return Assignment::defaultName;
}

const std::string Assignment::defaultName = "=";
