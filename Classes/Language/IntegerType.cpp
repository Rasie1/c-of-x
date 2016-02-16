#include "IntegerType.h"
#include "Integer.h"
#include "Void.h"

bool IntegerType::holds(ExpPtrArg e, Environment* env) const
{
    auto x = d_cast<Integer>(e);
    return x != nullptr;
}

std::string IntegerType::show() const
{
    return defaultName;
}

const std::string IntegerType::defaultName = "int";
