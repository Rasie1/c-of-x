#include "More.h"
#include "Equality.h"
#include "Identifier.h"
#include "Environment.h"
#include "Operation.h"
#include "Intersection.h"
#include "Void.h"
#include "Lambda.h"
#include "Integer.h"


bool MoreThan::holds(ExpPtrArg e, const Environment& env) const
{
    if (checkType<Integer>(e))
    {
        auto v1 = s_cast<Integer>(value);
        auto v2 = s_cast<Integer>(e);
        return v2->value > v1->value;
    }
    return *value == *e;
}

bool MoreThan::operator==(const Expression& other) const
{
    if (typeid(*this) == typeid(other))
    {
        auto x = static_cast<const MoreThan&>(other);
        return value == x.value;
    }
    else
        return false;
}

std::string MoreThan::show() const
{
    return defaultName + "(" + value->show() + ")";
}

const std::string MoreThan::defaultName = "moreThan";

ExpPtr MoreThan::intersect(ExpPtrArg other, const Environment& env)
{
    if (checkType<MoreThan>(other))
    {
        auto p = s_cast<MoreThan>(other);
        if (checkType<Integer>(value) && checkType<Identifier>(p->value))
        {
            auto v1 = s_cast<Integer>(value);
            auto v2 = s_cast<Integer>(p->value);
            return make_ptr<MoreThan>(make_ptr<Integer>(std::min(v1->value, v2->value)));
        }
        return make_ptr<Void>();
    }
    if (checkType<Integer>(other))
    {
        return shared_from_this();
    }
    return make_ptr<Operation>(make_ptr<Intersection>(), shared_from_this(), other);
}

ExpPtr MoreThan::begin()
{
    return value;
}

More::More()
    : Operator(true, 2)
{

}

ExpPtr More::operate(ExpPtrArg first,
                     ExpPtrArg second,
                     Environment& env) const
{
    auto f = s_cast<MoreThan>(partialApply(second, env));

    return f->apply(first, env);
}

std::string More::show() const
{
    return More::defaultName;
}

const std::string More::defaultName = ">";

ExpPtr More::partialApply(ExpPtrArg e, Environment& env) const
{
     return make_ptr<MoreThan>(e);
}
