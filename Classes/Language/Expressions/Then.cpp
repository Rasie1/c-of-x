#include "Then.h"
#include "Operation.h"
#include "Environment.h"
#include "Identifier.h"
#include "Void.h"

Then::Then()
    : Operator(true, 3)
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

const std::string Then::defaultName = "~";// "=>", later