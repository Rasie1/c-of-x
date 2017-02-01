#include "Expressions/Let.h"
#include "System/Environment.h"

ExpPtr Let::apply(ExpPtrArg e, Environment& env) const
{
    return e;
}

std::string Let::show() const
{
    return defaultName;
}

const std::string Let::defaultName = "let";

