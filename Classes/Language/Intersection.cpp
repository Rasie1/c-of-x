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
                         Environment*& env) const
{
    auto l = first->eval(env);
    auto r = second->eval(env);

    if (*(l->intersect(r, env)) == *(r->intersect(l, env)))
        return r;
    else
        return make_ptr<Void>();
}

std::string Intersection::show() const
{
    return Intersection::defaultName;
}

const std::string Intersection::defaultName = "&";
