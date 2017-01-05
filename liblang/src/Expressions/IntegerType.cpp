#include "Expressions/IntegerType.h"
#include "Expressions/Integer.h"
#include "Expressions/Void.h"
#include "Expressions/Identifier.h"
#include "System/Environment.h"

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