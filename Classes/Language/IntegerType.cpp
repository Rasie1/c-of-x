#include "IntegerType.h"
#include "Integer.h"
#include "Void.h"
#include "Identifier.h"

bool IntegerType::holds(ExpPtrArg e, Environment* env) const
{
    auto x = d_cast<Integer>(Identifier::unwrapIfId(e->eval(env), env));
    return x != nullptr;
}

std::string IntegerType::show() const
{
    return defaultName;
}

const std::string IntegerType::defaultName = "int";
