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

bool Equals::operator==(const Expression& other) const
{
    try
    {
        auto x = dynamic_cast<const Equals&>(other);
        return value == x.value;
    }
    catch (std::bad_cast&)
    {
        return false;
    }
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
        auto& l = *value;
        auto& r = *(p->value);
        if (l == r)
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
