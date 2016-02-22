#include "Any.h"

std::string Any::show() const
{
    return defaultName;
}

const std::string Any::defaultName = "_";


ExpPtr Any::intersect(ExpPtrArg other, Environment *env)
{
    return other;
}
