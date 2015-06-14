#include "PatternPredicate.h"
#include "Application.h"
#include "Integer.h"
#include "Environment.h"

namespace Language
{

PatternPredicate::PatternPredicate(Expression* predicate)
    : predicate(predicate)
{

}

bool PatternPredicate::match(Pattern* other, Environment* env)
{
    auto argument = env->get(other);
    auto result  = predicate->applyConstEnv(argument, env);
    auto integer = dynamic_cast<Integer*>(result);
    if (integer)
        return integer->value != 0;

    return false;
}

std::string PatternPredicate::toString() const
{
    return "(" + predicate->toString() + ") ?";
}

}
