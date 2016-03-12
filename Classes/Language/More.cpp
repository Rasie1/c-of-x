#include "More.h"
#include "Equality.h"
#include "Identifier.h"
#include "Environment.h"
#include "Operation.h"
#include "Intersection.h"
#include "Union.h"
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
        if (checkType<Integer>(value) && checkType<Integer>(Identifier::unwrapIfId(p->value, env)))
        {
            auto v1 = s_cast<Integer>(value);
            auto v2 = s_cast<Integer>(p->value);
            return make_ptr<MoreThan>(make_ptr<Integer>(std::max(v1->value, v2->value)));
        }
        return make_ptr<Void>();
    }
    else if (checkType<Equals>(other))
    {
        auto p = s_cast<Equals>(other);
        if (checkType<Integer>(p->value))
        {
            auto eqvalue = s_cast<Integer>(p->value)->value;
            auto thvalue = s_cast<Integer>(this->value)->value;
            if (thvalue < eqvalue)
                return p;
            else
                return make_ptr<Void>();
        }
    }

    return make_ptr<Operation>(make_ptr<Intersection>(), shared_from_this(), other);
}

ExpPtr MoreThan::unionize(ExpPtrArg other, const Environment& env)
{
    if (checkType<MoreThan>(other))
    {
        auto p = s_cast<MoreThan>(other);
        if (checkType<Integer>(value) && checkType<Integer>(Identifier::unwrapIfId(p->value, env)))
        {
            auto v1 = s_cast<Integer>(value);
            auto v2 = s_cast<Integer>(p->value);
            return make_ptr<MoreThan>(make_ptr<Integer>(std::min(v1->value, v2->value)));
        }
    }
    return make_ptr<Operation>(make_ptr<Union>(), shared_from_this(), other);
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
