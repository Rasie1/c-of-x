#include "Less.h"
#include "Equality.h"
#include "Identifier.h"
#include "Environment.h"
#include "Operation.h"
#include "Intersection.h"
#include "Void.h"
#include "Lambda.h"
#include "Integer.h"
#include "Union.h"


bool LessThan::holds(ExpPtrArg e, const Environment& env) const
{
    if (checkType<Integer>(e))
    {
        auto v1 = s_cast<Integer>(value);
        auto v2 = s_cast<Integer>(e);
        return v2->value < v1->value;
    }
    return *value == *e;
}

bool LessThan::operator==(const Expression& other) const
{
    if (typeid(*this) == typeid(other))
    {
        auto x = static_cast<const LessThan&>(other);
        return value == x.value;
    }
    else
        return false;
}

std::string LessThan::show() const
{
    return defaultName + "(" + value->show() + ")";
}

const std::string LessThan::defaultName = "lessThan";

ExpPtr LessThan::intersect(ExpPtrArg other, const Environment& env)
{
    if (checkType<LessThan>(other))
    {
        auto p = s_cast<LessThan>(other);
        if (checkType<Integer>(value) && checkType<Integer>(Identifier::unwrapIfId(p->value, env)))
        {
            auto v1 = s_cast<Integer>(value);
            auto v2 = s_cast<Integer>(p->value);
            return make_ptr<LessThan>(make_ptr<Integer>(std::min(v1->value, v2->value)));
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
            if (thvalue > eqvalue)
                return p;
            else
                return make_ptr<Void>();
        }
    }

    return make_ptr<Operation>(make_ptr<Intersection>(), shared_from_this(), other);
}

ExpPtr LessThan::unionize(ExpPtrArg other, const Environment& env)
{
    if (checkType<LessThan>(other))
    {
        auto p = s_cast<LessThan>(other);
        if (checkType<Integer>(value) && checkType<Integer>(Identifier::unwrapIfId(p->value, env)))
        {
            auto v1 = s_cast<Integer>(value);
            auto v2 = s_cast<Integer>(p->value);
            return make_ptr<LessThan>(make_ptr<Integer>(std::max(v1->value, v2->value)));
        }
    }

    return make_ptr<Operation>(make_ptr<Union>(), shared_from_this(), other);
}

ExpPtr LessThan::end()
{
    return value;
}

Less::Less()
    : Operator(true, 2)
{

}

ExpPtr Less::operate(ExpPtrArg first,
                     ExpPtrArg second,
                     Environment& env) const

{
    auto f = s_cast<LessThan>(partialApply(second, env));

    return f->apply(first, env);
}

std::string Less::show() const
{
    return Less::defaultName;
}

const std::string Less::defaultName = "<";

ExpPtr Less::partialApply(ExpPtrArg e, Environment& env) const
{
     return make_ptr<LessThan>(e);
}
