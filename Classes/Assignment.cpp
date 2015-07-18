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
    ExpPtr lvalue = first;
    ExpPtr rvalue = second;
    while (lvalue->unwind(lvalue, rvalue, env));
    ExpPtr evaluated = rvalue->eval(env);
    env = env->add(lvalue->pattern(), evaluated);
    //auto unwinded  = first->unwind(env);
    //auto evaluated = second->eval(env);
    //env = env->add(unwinded.first,
    //               unwinded.second == PatPtr()
    //               ? evaluated
    //               : Lambda::construct(unwinded.second,
    //                                   evaluated,
    //                                   env));
    return evaluated;
}

std::string Assignment::toString() const
{
    return Assignment::defaultName;
}

const std::string Assignment::defaultName = "=";

}
