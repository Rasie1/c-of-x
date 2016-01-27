#include "Assignment.h"
#include "Identifier.h"
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
    ExpPtr lvalue, rvalue;
    lvalue = first;

    if (d_cast<Identifier>(second))
        rvalue = env->get(second);
    if (rvalue == nullptr)
        rvalue = second;

    if (d_cast<Identifier>(lvalue))
        env = env->add(lvalue, rvalue);
    else
        while (lvalue->unwind(lvalue, rvalue, env));

    // Enable recursion
    auto operation = d_cast<Operation>(rvalue);
    if (operation && d_cast<Lambda>(operation->op))
        env = env->add(lvalue, rvalue);

    return second;
}

std::string Assignment::show() const
{
    return Assignment::defaultName;
}

const std::string Assignment::defaultName = "=";