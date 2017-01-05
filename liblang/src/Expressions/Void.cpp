#include "Expressions/Void.h"


ExpPtr Void::intersect(ExpPtrArg other, const Environment& env)
{
    return shared_from_this();
}

std::string Void::show() const
{
    return defaultName;
}

const std::string Void::defaultName = "void";

bool Void::unapplyVariables(ExpPtrArg e, Environment& env) const
{
    return false;
}
