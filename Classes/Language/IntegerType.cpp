#include "IntegerType.h"
#include "Integer.h"
#include "Void.h"
#include "Identifier.h"
#include "Environment.h"

bool IntegerType::holds(ExpPtrArg e, const Environment& env) const
{
    auto envc = env;
    return typeid(*(Identifier::unwrapIfId(e->eval(envc), envc))) == typeid(Integer);
}

std::string IntegerType::show() const
{
    return defaultName;
}

const std::string IntegerType::defaultName = "int";
