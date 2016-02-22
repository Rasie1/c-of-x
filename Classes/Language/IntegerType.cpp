#include "IntegerType.h"
#include "Integer.h"
#include "Void.h"
#include "Identifier.h"

bool IntegerType::holds(ExpPtrArg e, Environment* env) const
{
    return typeid(*(Identifier::unwrapIfId(e->eval(env), env))) == typeid(Integer);
}

std::string IntegerType::show() const
{
    return defaultName;
}

const std::string IntegerType::defaultName = "int";
