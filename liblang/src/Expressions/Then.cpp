#include "Expressions/Then.h"
#include "Expressions/Operation.h"
#include "System/Environment.h"
#include "Expressions/Identifier.h"
#include "Expressions/Void.h"
#include "System/Cast.h"

Then::Then()
    : Operator(true, 3, true)
{
}

Object Then::operate(const Object& first,
                     const Object& second,
                     Environment& env)
{
    auto l = first->eval(env);
    if (checkType<Void>(l))
        return l;

    return second->eval(env);
}

std::string Then::show() const
{
    return Then::defaultName;
}

const std::string Then::defaultName = "==>";
