#include "Equality.h"
#include "Identifier.h"
#include "Environment.h"
#include "Operation.h"
#include "Intersection.h"
#include "Void.h"

bool Equals::holds(ExpPtrArg e, Environment* env) const
{
    return *value == *e;
}

std::string Equals::show() const
{
    return defaultName + "(" + value->show() + ")";
}

const std::string Equals::defaultName = "eqv";

ExpPtr Equals::intersect(ExpPtrArg other, Environment* env)
{
    if (std::shared_ptr<Equals> p = d_cast<Equals>(other))
    {
        if (*value == *p->value)
            return value;
        else
            return make_ptr<Void>();
    }
    return make_ptr<Operation>(make_ptr<Intersection>(), value, other);
}

ExpPtr Equality::apply(ExpPtrArg e, Environment*& env) const
{
    return make_ptr<Equals>(e);
}

std::string Equality::show() const
{
    return defaultName;
}

const std::string Equality::defaultName = "eq";
