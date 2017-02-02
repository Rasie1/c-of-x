#include "Expressions/Let.h"
#include "Expressions/Any.h"
#include "Expressions/Void.h"
#include "System/Environment.h"

ExpPtr Let::apply(ExpPtrArg e, Environment& env) const
{
    // TODO: add new value to env unionized with old value
    auto result = e->eval(env);
    if (checkType<Void>(result))
        return result;
    else
        return make_ptr<Any>();
}

std::string Let::show() const
{
    return defaultName;
}

const std::string Let::defaultName = "let";

