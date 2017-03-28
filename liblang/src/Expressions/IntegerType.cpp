#include "Expressions/IntegerType.h"
#include "Expressions/Integer.h"
#include "Expressions/Void.h"
#include "Expressions/Identifier.h"
#include "System/Environment.h"
#include "System/Cast.h"

optional<bool> IntegerType::holds(const Object& e, Environment& env)
{
    auto ret = checkType<Integer>(env, e);
    return make_optional(ret);
}

std::string IntegerType::show() const
{
    return defaultName;
}

const std::string IntegerType::defaultName = "int";
