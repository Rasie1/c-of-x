#include "Function.h"
#include "Environment.h"
#include "Void.h"
#include "Equality.h"
#include "Any.h"
#include "Identifier.h"
#include "String.h"

ExpPtr Function::intersect(ExpPtrArg other, const Environment& env)
{
    auto val = Identifier::unwrapIfId(other, env);
    if (typeid(*this) == typeid(*val))
        return val;
    if (checkType<Any>(val))
    {
        return make_ptr<String>("lolo");
    }
    if (checkType<Equals>(val))
    {
        auto eq = s_cast<Equals>(other);
        auto ev = eq->value;//->eval(env);
        auto envc = env;
        auto ret = apply(ev, envc);
        if (checkType<Void>(ret))
            return ret;
        return make_ptr<Equals>(ret);
    }

    auto ret = make_ptr<String>("Wrong argument");

    return ret;
}

bool ReversibleFunction::unapplyVariables(ExpPtrArg e, ExpPtrArg arg, Environment& env) const
{
    auto reversed = reverse()->apply(arg, env);

    auto ret = reversed->unapplyVariables(e, env);

    return ret;
}
