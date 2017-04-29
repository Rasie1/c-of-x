#include "Expressions/Any.h"

std::string Any::show() const
{
    return defaultName;
}

Object Any::apply(const Object& e, Environment& env)
{
    return makeObject<Any>();
}

const std::string Any::defaultName = "_";

bool Any::unapplyVariables(const Object& e, Environment& env)
{
    return true;
}

Object Any::intersect(const Object& other, Environment& env)
{
    return other;
}
