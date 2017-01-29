#include "Expressions/Any.h"

std::string Any::show() const
{
    return defaultName;
}

const std::string Any::defaultName = "_";

bool Any::unapplyVariables(ExpPtrArg e, Environment& env) const
{
    return true;
}

ExpPtr Any::intersect(ExpPtrArg other, const Environment& env)
{
    return other;
}
