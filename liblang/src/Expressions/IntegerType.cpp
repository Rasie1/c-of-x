#include "Expressions/IntegerType.h"
#include "Expressions/Integer.h"
#include "Expressions/Void.h"
#include "Expressions/Identifier.h"
#include "Expressions/ValueInSet.h"
#include "System/Environment.h"
#include "System/Cast.h"

optional<bool> IntegerType::holds(const Object& e, Environment& env)
{
    if (auto i = cast<Integer>(env, e))
    {
        return make_optional(true);
    }
    if (auto valueInSet = cast<ValueInSet>(env, e))
    if (auto integerType = cast<IntegerType>(env, valueInSet->set))
    {
        return make_optional(true);
    }

    return make_optional(false);
}

std::string IntegerType::show() const
{
    return defaultName;
}

const std::string IntegerType::defaultName = "int";
