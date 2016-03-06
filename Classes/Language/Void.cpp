#include "Void.h"

std::string Void::show() const
{
    return defaultName;
}

const std::string Void::defaultName = "void";

bool Void::unapplyVariables(ExpPtrArg e, Environment& env) const
{
    return false;
}
