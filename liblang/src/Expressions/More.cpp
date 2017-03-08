#include "Expressions/More.h"
#include "Expressions/Equality.h"
#include "Expressions/Identifier.h"
#include "System/Environment.h"
#include "Expressions/Operation.h"
#include "Expressions/Intersection.h"
#include "Expressions/Union.h"
#include "Expressions/Void.h"
#include "Expressions/Lambda.h"
#include "Expressions/Integer.h"
#include "Expressions/Application.h"
#include "Expressions/Less.h"


optional<bool> MoreThan::holds(ExpPtrArg e, const Environment& env) const
{
    if (auto v1 = d_cast<Integer>(value))
    if (auto v2 = d_cast<Integer>(e))
    {
        return make_optional(v2->value > v1->value);
    }
    return none;
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
    if (auto p = d_cast<MoreThan>(other))
    {
        if (auto v1 = d_cast<Integer>(value))
        if (auto v2 = d_cast<Integer>(Identifier::unwrapIfId(p->value, env)))
        {
            return make_ptr<MoreThan>(make_ptr<Integer>(std::max(v1->value, v2->value)));
        }
        return make_ptr<Void>();
    }
    else if (auto p = d_cast<Equals>(other))
    {
        if (auto v1 = d_cast<Integer>(p->value))
        if (auto v2 = d_cast<Integer>(this->value))
        {
            auto eqvalue = v1->value;
            auto thvalue = v2->value;
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
    if (auto p = d_cast<MoreThan>(other))
    {
        if (auto v1 = d_cast<Integer>(value))
        if (auto v2 = d_cast<Integer>(Identifier::unwrapIfId(p->value, env)))
        {
            return make_ptr<MoreThan>(make_ptr<Integer>(std::min(v1->value, v2->value)));
        }
    }
    return make_ptr<Operation>(make_ptr<Union>(), shared_from_this(), other);
}

ExpPtr MoreThan::complement(const Environment& env)
{
    return make_ptr<LessOrEqualThan>(value);
}

ExpPtr MoreThan::begin()
{
    return value;
}

More::More()
    : Operator(true, 2, true)
{

}

ExpPtr More::operate(ExpPtrArg first,
                     ExpPtrArg second,
                     Environment& env) const
{
    auto f = d_cast<MoreThan>(partialApplyRight(second, env));

    return Application().operate(f, first, env);
}

std::string More::show() const
{
    return More::defaultName;
}

const std::string More::defaultName = ">";

ExpPtr More::partialApplyLeft(ExpPtrArg e, Environment& env) const
{
     return make_ptr<LessThan>(e);
}

ExpPtr More::partialApplyRight(ExpPtrArg e, Environment& env) const
{
     return make_ptr<MoreThan>(e);
}

optional<bool> MoreOrEqualThan::holds(ExpPtrArg e, const Environment& env) const
{
    if (auto v1 = d_cast<Integer>(value))
    if (auto v2 = d_cast<Integer>(e))
    {
        return make_optional(v2->value >= v1->value);
    }
    return none;
}

bool MoreOrEqualThan::operator==(const Expression& other) const
{
    if (typeid(*this) == typeid(other))
    {
        auto x = static_cast<const MoreOrEqualThan&>(other);
        return value == x.value;
    }
    else
        return false;
}

std::string MoreOrEqualThan::show() const
{
    return defaultName + "(" + value->show() + ")";
}

const std::string MoreOrEqualThan::defaultName = "moreOrEqualThan";

ExpPtr MoreOrEqualThan::intersect(ExpPtrArg other, const Environment& env)
{
    if (auto p = d_cast<MoreOrEqualThan>(other))
    {
        if (auto v1 = d_cast<Integer>(value))
        if (auto v2 = d_cast<Integer>(Identifier::unwrapIfId(p->value, env)))
        {
            return make_ptr<MoreOrEqualThan>(make_ptr<Integer>(std::max(v1->value, v2->value)));
        }
        return make_ptr<Void>();
    }
    else if (auto d = d_cast<Equals>(other))
    {
        if (auto v1 = d_cast<Integer>(p->value))
        if (auto v2 = d_cast<Integer>(this->value))
        {
            auto eqvalue = v1->value;
            auto thvalue = v2->value;
            if (thvalue < eqvalue)
                return p;
            else
                return make_ptr<Void>();
        }
    }

    return make_ptr<Operation>(make_ptr<Intersection>(), shared_from_this(), other);
}

ExpPtr MoreOrEqualThan::unionize(ExpPtrArg other, const Environment& env)
{
    if (auto p = d_cast<MoreOrEqualThan>(other))
    {
        if (auto v1 = d_cast<Integer>(value))
        if (auto v2 = d_cast<Integer>(Identifier::unwrapIfId(p->value, env)))
        {
            return make_ptr<MoreOrEqualThan>(make_ptr<Integer>(std::min(v1->value, v2->value)));
        }
    }
    return make_ptr<Operation>(make_ptr<Union>(), shared_from_this(), other);
}

ExpPtr MoreOrEqualThan::complement(const Environment& env)
{
    return make_ptr<LessThan>(value);
}

ExpPtr MoreOrEqualThan::begin()
{
    return value;
}

MoreOrEqual::MoreOrEqual()
    : Operator(true, 2, true)
{

}

ExpPtr MoreOrEqual::operate(ExpPtrArg first,
                     ExpPtrArg second,
                     Environment& env) const
{
    auto f = d_cast<MoreOrEqualThan>(partialApplyRight(second, env));

    return f->apply(first, env);
}

std::string MoreOrEqual::show() const
{
    return MoreOrEqual::defaultName;
}

const std::string MoreOrEqual::defaultName = ">=";

ExpPtr MoreOrEqual::partialApplyLeft(ExpPtrArg e, Environment& env) const
{
     return make_ptr<LessThan>(e);
}

ExpPtr MoreOrEqual::partialApplyRight(ExpPtrArg e, Environment& env) const
{
     return make_ptr<MoreThan>(e);
}
