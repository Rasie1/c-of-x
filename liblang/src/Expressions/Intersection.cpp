#include "Expressions/Intersection.h"
#include "Expressions/Operation.h"
#include "Expressions/Identifier.h"
#include "Expressions/Void.h"
#include "System/Environment.h"

Intersection::Intersection()
    : Operator(false, 4, true)
{
}

ExpPtr Intersection::operate(ExpPtrArg first,
                             ExpPtrArg second,
                             Environment& env) const
{
    env.debugPrint("Intersecting operation\n");
    env.increaseDebugIndentation();
    env.debugPrint("FST: " + first->show() + "\n");
    env.debugPrint("SND: " + second->show() + "\n");
    env.decreaseDebugIndentation();
    auto l = Identifier::unwrapIfId(first, env)->eval(env);
    auto r = Identifier::unwrapIfId(second, env)->eval(env);

    if (*l == *r)
        return l;

    auto lr = l->intersect(r, env);

    bool lf = false;
    bool rf = false;
    if (checkType<Operation>(lr))
    {
        auto op = s_cast<Operation>(lr)->op;
        if (checkType<Intersection>(op))
            lf = true;
    }
    if (!lf)
        return lr;

    auto rl = r->intersect(l, env);

    if (checkType<Operation>(rl))
    {
        auto op = s_cast<Operation>(rl)->op;
        if (checkType<Intersection>(op))
            rf = true;
    }
    if (!rf)
        return rl;

    // TODO: make this default behaviour and remove returns for both
    // branches (lines 40-41 and 51-52)
    // if (!lf || !rf)
    //     return make_ptr<Void>();

    return lr;
}

std::string Intersection::show() const
{
    return Intersection::defaultName;
}

const std::string Intersection::defaultName = "&";

bool Intersection::unapplyVariables(ExpPtrArg e,
                                    ExpPtrArg l,
                                    ExpPtrArg r,
                                    Environment &env) const
{
    auto lUnapplied = l->unapplyVariables(e, env);
    auto rUnapplied = r->unapplyVariables(e, env);

    auto result = lUnapplied && rUnapplied;

    return result;
}

ExpPtr Intersection::apply(ExpPtrArg l, ExpPtrArg r, ExpPtrArg e, Environment &env) const 
{
    auto envc = env;
    auto lApplied = l->apply(e, env);
    auto rApplied = r->apply(e, env);

    auto result = Intersection().operate(lApplied, rApplied, env);

    return result;
}
