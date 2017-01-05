#include "IntegerType.h"
#include "Integer.h"
#include "Void.h"
#include "Identifier.h"
#include "Environment.h"

bool IntegerType::holds(ExpPtrArg e, const Environment& env) const
{
    auto envc = env;
    auto value = Identifier::unwrapIfId(e->eval(envc), envc);
    auto ret = typeid(*value) == typeid(Integer);
    return ret;
}

std::string IntegerType::show() const
{
    return defaultName;
}

const std::string IntegerType::defaultName = "int";
