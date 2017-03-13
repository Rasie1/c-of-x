#include "Expressions/Less.h"
#include "Expressions/Equality.h"
#include "Expressions/Identifier.h"
#include "System/Environment.h"
#include "Expressions/Operation.h"
#include "Expressions/Intersection.h"
#include "Expressions/Void.h"
#include "Expressions/Lambda.h"
#include "Expressions/Integer.h"
#include "Expressions/Union.h"
#include "Expressions/More.h"
#include "Expressions/ValueInSet.h"


optional<bool> LessThan::holds(ExpPtrArg e, const Environment& env)
{
    if (auto v1 = d_cast<Integer>(value))
    if (auto v2 = d_cast<Integer>(e))
    {
        return make_optional(v2->value < v1->value);
    }
    return none;
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
    if (auto p = d_cast<LessThan>(other))
    {
        if (auto v1 = d_cast<Integer>(value))
        if (auto v2 = d_cast<Integer>(Identifier::unwrapIfId(p->value, env)))
        {
            return make_ptr<LessThan>(make_ptr<Integer>(std::min(v1->value, v2->value)));
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
    if (auto p = d_cast<LessThan>(other))
    {
        if (auto v1 = d_cast<Integer>(value))
        if (auto v2 = d_cast<Integer>(Identifier::unwrapIfId(p->value, env)))
        {
            return make_ptr<LessThan>(make_ptr<Integer>(std::max(v1->value, v2->value)));
        }
    }

    return make_ptr<Operation>(make_ptr<Union>(), shared_from_this(), other);
}

ExpPtr LessThan::complement(const Environment& env)
{
    return make_ptr<MoreOrEqualThan>(value);
}

ExpPtr LessThan::end()
{
    return value;
}

Less::Less()
    : Operator(true, 2, true)
{

}

ExpPtr Less::operate(ExpPtrArg first,
                     ExpPtrArg second,
                     Environment& env)

{
    auto f = d_cast<LessThan>(partialApplyRight(second, env));

    return f->apply(first, env);
}

std::string Less::show() const
{
    return Less::defaultName;
}

const std::string Less::defaultName = "<";

ExpPtr Less::partialApplyLeft(ExpPtrArg e, Environment& env)
{
     return make_ptr<MoreThan>(e);
}

ExpPtr Less::partialApplyRight(ExpPtrArg e, Environment& env)
{
     return make_ptr<LessThan>(e);
}


optional<bool> LessOrEqualThan::holds(ExpPtrArg e, const Environment& env)
{
    if (auto v1 = d_cast<Integer>(value))
    if (auto v2 = d_cast<Integer>(e))
    {
        return make_optional(v2->value <= v1->value);
    }
    return none;
}

bool LessOrEqualThan::operator==(const Expression& other) const
{
    if (typeid(*this) == typeid(other))
    {
        auto x = static_cast<const LessOrEqualThan&>(other);
        return value == x.value;
    }
    else
        return false;
}

std::string LessOrEqualThan::show() const
{
    return defaultName + "(" + value->show() + ")";
}

const std::string LessOrEqualThan::defaultName = "lessOrEqualThan";

ExpPtr LessOrEqualThan::intersect(ExpPtrArg other, const Environment& env)
{
    if (auto p = d_cast<LessOrEqualThan>(other))
    {
        if (auto v1 = d_cast<Integer>(value))
        if (auto v2 = d_cast<Integer>(Identifier::unwrapIfId(p->value, env)))
        {
            return make_ptr<LessOrEqualThan>(make_ptr<Integer>(std::min(v1->value, v2->value)));
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
            if (thvalue > eqvalue)
                return p;
            else
                return make_ptr<Void>();
        }
    }

    return make_ptr<Operation>(make_ptr<Intersection>(), shared_from_this(), other);
}

ExpPtr LessOrEqualThan::unionize(ExpPtrArg other, const Environment& env)
{
    if (auto p = d_cast<LessOrEqualThan>(other))
    {
        if (auto v1 = d_cast<Integer>(value))
        if (auto v2 = d_cast<Integer>(Identifier::unwrapIfId(p->value, env)))
        {
            return make_ptr<LessOrEqualThan>(make_ptr<Integer>(std::max(v1->value, v2->value)));
        }
    }

    return make_ptr<Operation>(make_ptr<Union>(), shared_from_this(), other);
}

ExpPtr LessOrEqualThan::complement(const Environment& env)
{
    return make_ptr<MoreThan>(value);
}

ExpPtr LessOrEqualThan::end()
{
    return value;
}

LessOrEqual::LessOrEqual()
    : Operator(true, 2, true)
{

}

ExpPtr LessOrEqual::operate(ExpPtrArg first,
                     ExpPtrArg second,
                     Environment& env)

{
    auto f = d_cast<LessOrEqualThan>(partialApplyRight(second, env));

    return f->apply(first, env);
}

std::string LessOrEqual::show() const
{
    return LessOrEqual::defaultName;
}

const std::string LessOrEqual::defaultName = "=<";

ExpPtr LessOrEqual::partialApplyLeft(ExpPtrArg e, Environment& env)
{
     return make_ptr<MoreOrEqualThan>(e);
}

ExpPtr LessOrEqual::partialApplyRight(ExpPtrArg e, Environment& env)
{
     return make_ptr<LessOrEqualThan>(e);
}
