#include "Expressions/Any.h"

std::string Any::show() const
{
    return defaultName;
}

const std::string Any::defaultName = "_";


ExpPtr Any::intersect(ExpPtrArg other, const Environment& env)
{
    return other;
}
