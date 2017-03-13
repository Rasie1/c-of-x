#include "Expressions/Any.h"

std::string Any::show() const
{
    return defaultName;
}

ExpPtr Any::apply(ExpPtrArg e, Environment& env)
{
    return make_ptr<Any>();
}

const std::string Any::defaultName = "_";

bool Any::unapplyVariables(ExpPtrArg e, Environment& env)
{
    return true;
}

ExpPtr Any::intersect(ExpPtrArg other, const Environment& env)
{
    return other;
}
