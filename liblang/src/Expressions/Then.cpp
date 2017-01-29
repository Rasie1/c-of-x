#include "Expressions/Then.h"
#include "Expressions/Operation.h"
#include "System/Environment.h"
#include "Expressions/Identifier.h"
#include "Expressions/Void.h"

Then::Then()
    : Operator(true, 3, true)
{
}

ExpPtr Then::operate(ExpPtrArg first,
                     ExpPtrArg second,
                     Environment& env) const
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
