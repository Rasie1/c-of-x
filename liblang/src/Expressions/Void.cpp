#include "Expressions/Void.h"


Object Void::intersect(const Object& other, const Environment& env)
{
    return thisObject();
}

std::string Void::show() const
{
    return defaultName;
}

const std::string Void::defaultName = "void";

bool Void::unapplyVariables(const Object& e, Environment& env)
{
    return false;
}
