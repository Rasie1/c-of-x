#include "PatternPredicate.h"
#include "Application.h"
#include "Integer.h"
#include "Environment.h"

PatternPredicate::PatternPredicate(Expression* predicate)
    : predicate(predicate)
{

}

bool PatternPredicate::match(ExpPtrArg other) const
{
    /*auto argument = env->get(other);
    auto newEnv = env;
    auto result  = predicate->apply(argument, newEnv);
    auto integer = d_cast<Integer>(result);
    if (integer)
        return integer->value != 0;*/

    return false;
}

std::string PatternPredicate::show() const
{
    return "(" + predicate->show() + ") ?";
}

