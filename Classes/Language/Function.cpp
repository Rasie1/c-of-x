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
        return ret;
    }

    auto ret = make_ptr<String>("Wrong argument");

    return ret;

//    auto env = envc;
//    ExpPtr val;
//    if (checkType<Equals>(other))
//    {
//        auto eq = s_cast<Equals>(other);
//        auto ev = eq->value;//->eval(env);
//        return apply(ev, env);
//    }
//    else
//    {
//        auto val = Identifier::unwrapIfId(other, env);
//        if (checkType<Any>(val))
//            return make_ptr<Any>();
//        else
//            return apply(val, env);
//    }
}

//void Function::unapplyVariables(ExpPtrArg e, Environment& env) const
//{
//    auto r = s_cast<Function>(reverse());
//    return r->apply(e, env);
//}

bool ReversibleFunction::unapplyVariables(ExpPtrArg e, ExpPtrArg arg, Environment& env) const
{
    return reverse()->apply(arg, env)->unapplyVariables(e, env);
}
