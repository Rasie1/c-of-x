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
                     Environment*& env) const
{
    auto l = first->eval(env);
    auto r = second->eval(env);

    if (d_cast<Void>(l))
        return make_ptr<Void>();

    return r;
}

std::string Then::show() const
{
    return Then::defaultName;
}

const std::string Then::defaultName = "=>";

bool Then::unwind(ExpPtr& left,
                  ExpPtr& right,
                  ExpPtr& lvalue,
                  ExpPtr& rvalue,
                  Environment*& env)
{
    return false;
}