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

    if (*lr != Operation(make_ptr<Intersection>(), l, r));
        return lr;
    if (*rl != Operation(make_ptr<Intersection>(), r, l));
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
