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
#include "System/Cast.h"


optional<bool> LessThan::holds(const Object& e, const Environment& env)
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

Object LessThan::intersect(const Object& other, const Environment& env)
{
    if (auto p = d_cast<LessThan>(other))
    {
        if (auto v1 = d_cast<Integer>(value))
        if (auto v2 = d_cast<Integer>(Identifier::unwrapIfId(p->value, env)))
        {
            return makeObject<LessThan>(makeObject<Integer>(std::min(v1->value, v2->value)));
        }
        return makeObject<Void>();
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
                return makeObject<Void>();
        }
    }

    return makeObject<Operation>(makeObject<Intersection>(), shared_from_this(), other);
}

Object LessThan::unionize(const Object& other, const Environment& env)
{
    if (auto p = d_cast<LessThan>(other))
    {
        if (auto v1 = d_cast<Integer>(value))
        if (auto v2 = d_cast<Integer>(Identifier::unwrapIfId(p->value, env)))
        {
            return makeObject<LessThan>(makeObject<Integer>(std::max(v1->value, v2->value)));
        }
    }

    return makeObject<Operation>(makeObject<Union>(), shared_from_this(), other);
}

Object LessThan::complement(const Environment& env)
{
    return makeObject<MoreOrEqualThan>(value);
}

Object LessThan::end()
{
    return value;
}

Less::Less()
    : Operator(true, 2, true)
{

}

Object Less::operate(const Object& first,
                     const Object& second,
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

Object Less::partialApplyLeft(const Object& e, Environment& env)
{
     return makeObject<MoreThan>(e);
}

Object Less::partialApplyRight(const Object& e, Environment& env)
{
     return makeObject<LessThan>(e);
}


optional<bool> LessOrEqualThan::holds(const Object& e, const Environment& env)
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

Object LessOrEqualThan::intersect(const Object& other, const Environment& env)
{
    if (auto p = d_cast<LessOrEqualThan>(other))
    {
        if (auto v1 = d_cast<Integer>(value))
        if (auto v2 = d_cast<Integer>(Identifier::unwrapIfId(p->value, env)))
        {
            return makeObject<LessOrEqualThan>(makeObject<Integer>(std::min(v1->value, v2->value)));
        }
        return makeObject<Void>();
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
                return makeObject<Void>();
        }
    }

    return makeObject<Operation>(makeObject<Intersection>(), shared_from_this(), other);
}

Object LessOrEqualThan::unionize(const Object& other, const Environment& env)
{
    if (auto p = d_cast<LessOrEqualThan>(other))
    {
        if (auto v1 = d_cast<Integer>(value))
        if (auto v2 = d_cast<Integer>(Identifier::unwrapIfId(p->value, env)))
        {
            return makeObject<LessOrEqualThan>(makeObject<Integer>(std::max(v1->value, v2->value)));
        }
    }

    return makeObject<Operation>(makeObject<Union>(), shared_from_this(), other);
}

Object LessOrEqualThan::complement(const Environment& env)
{
    return makeObject<MoreThan>(value);
}

Object LessOrEqualThan::end()
{
    return value;
}

LessOrEqual::LessOrEqual()
    : Operator(true, 2, true)
{

}

Object LessOrEqual::operate(const Object& first,
                     const Object& second,
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

Object LessOrEqual::partialApplyLeft(const Object& e, Environment& env)
{
     return makeObject<MoreOrEqualThan>(e);
}

Object LessOrEqual::partialApplyRight(const Object& e, Environment& env)
{
     return makeObject<LessOrEqualThan>(e);
}
