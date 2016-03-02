#include "Function.h"
#include "Environment.h"
#include "Void.h"
#include "Equality.h"
#include "Any.h"
#include "Identifier.h"

ExpPtr Function::intersect(ExpPtrArg other, const Environment& envc)
{
    auto env = envc;
    ExpPtr val;
    if (typeid(*other) == typeid(Equals))
    {
        auto eq = s_cast<Equals>(other);
        auto ev = eq->value;//->eval(env);
        return apply(ev, env);
    }
    else
    {
        auto val = Identifier::unwrapIfId(other, env);
        if (typeid(*val) == typeid(Any))
            return make_ptr<Void>();
        else
            return apply(val, env);
    }
}

ExpPtr Function::reverse() const
{
    return make_ptr<Void>();
}

//void Function::unapplyVariables(ExpPtrArg e, Environment& env) const
//{
//    auto r = s_cast<Function>(reverse());
//    return r->apply(e, env);
//}
