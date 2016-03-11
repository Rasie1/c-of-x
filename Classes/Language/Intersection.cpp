#include "Intersection.h"
#include "Operation.h"
#include "Identifier.h"
#include "Void.h"

Intersection::Intersection()
    : Operator(false, 4)
{
}

ExpPtr Intersection::operate(ExpPtrArg first,
                             ExpPtrArg second,
                             Environment& env) const
{
    auto l = first->eval(env);
    auto r = second->eval(env);

    auto lr = l->intersect(r, env);
    auto rl = r->intersect(l, env);

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

    if (checkType<Operation>(rl))
    {
        auto op = s_cast<Operation>(rl)->op;
        if (checkType<Intersection>(op))
            rf = true;
    }
    if (!rf)
        return rl;

    return lr;

//    if (*rl == *lr)
//        return lr;
//    else
//        return make_ptr<Void>();
}

std::string Intersection::show() const
{
    return Intersection::defaultName;
}

const std::string Intersection::defaultName = "&";
