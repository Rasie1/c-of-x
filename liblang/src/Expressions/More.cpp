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
#include "System/Cast.h"


optional<bool> MoreThan::holds(const Object& e, Environment& envc)
{
    auto env = envc;
    if (auto v1 = cast<Integer>(env, value))
    if (auto v2 = cast<Integer>(env, e))
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

Object MoreThan::intersect(const Object& other, Environment& envc)
{
    auto env = envc;
    if (auto p = cast<MoreThan>(env, other))
    {
        if (auto v1 = cast<Integer>(env, value))
        if (auto v2 = cast<Integer>(env, Identifier::unwrapIfId(p->value, env)))
        {
            return makeObject<MoreThan>(makeObject<Integer>(std::max(v1->value, v2->value)));
        }
        return makeObject<Void>();
    }
    else if (auto p = cast<Equals>(env, other))
    {
        if (auto v1 = cast<Integer>(env, p->value))
        if (auto v2 = cast<Integer>(env, this->value))
        {
            auto eqvalue = v1->value;
            auto thvalue = v2->value;
            if (thvalue < eqvalue)
                return Object(p);
            else
                return makeObject<Void>();
        }
    }

    return makeOperation<Intersection>(thisObject(), other);
}

Object MoreThan::unionize(const Object& other, Environment& env)
{
    auto envc = env;
    if (auto p = cast<MoreThan>(envc, other))
    {
        if (auto v1 = cast<Integer>(envc, value))
        if (auto v2 = cast<Integer>(envc, Identifier::unwrapIfId(p->value, envc)))
        {
            return makeObject<MoreThan>(makeObject<Integer>(std::min(v1->value, v2->value)));
        }
    }
    return makeOperation<Union>(thisObject(), other);
}

Object MoreThan::complement(const Environment& env)
{
    return makeObject<LessOrEqualThan>(value);
}

Object MoreThan::begin()
{
    return value;
}

More::More()
    : Operator(true, 2, true)
{

}

Object More::operate(const Object& first,
                     const Object& second,
                     Environment& env)
{
    auto f = cast<MoreThan>(env, partialApplyRight(second, env));

    return Application().operate(Object(f), first, env);
}

std::string More::show() const
{
    return More::defaultName;
}

const std::string More::defaultName = ">";

Object More::partialApplyLeft(const Object& e, Environment& env)
{
     return makeObject<LessThan>(e);
}

Object More::partialApplyRight(const Object& e, Environment& env)
{
     return makeObject<MoreThan>(e);
}

optional<bool> MoreOrEqualThan::holds(const Object& e, Environment& envc)
{
    auto env = envc;
    if (auto v1 = cast<Integer>(env, value))
    if (auto v2 = cast<Integer>(env, e))
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

Object MoreOrEqualThan::intersect(const Object& other, Environment& envc)
{
    auto env = envc;
    if (auto p = cast<MoreOrEqualThan>(env, other))
    {
        if (auto v1 = cast<Integer>(env, value))
        if (auto v2 = cast<Integer>(env, Identifier::unwrapIfId(p->value, env)))
        {
            return makeObject<MoreOrEqualThan>(makeObject<Integer>(std::max(v1->value, v2->value)));
        }
        return makeObject<Void>();
    }
    else if (auto d = cast<Equals>(env, other))
    {
        if (auto v1 = cast<Integer>(env, p->value))
        if (auto v2 = cast<Integer>(env, this->value))
        {
            auto eqvalue = v1->value;
            auto thvalue = v2->value;
            if (thvalue < eqvalue)
                return Object(p);
            else
                return makeObject<Void>();
        }
    }

    return makeOperation<Intersection>(thisObject(), other);
}

Object MoreOrEqualThan::unionize(const Object& other, Environment& env)
{
    auto envc = env;
    if (auto p = cast<MoreOrEqualThan>(envc, other))
    {
        if (auto v1 = cast<Integer>(envc, value))
        if (auto v2 = cast<Integer>(envc, Identifier::unwrapIfId(p->value, envc)))
        {
            return makeObject<MoreOrEqualThan>(makeObject<Integer>(std::min(v1->value, v2->value)));
        }
    }
    return makeOperation<Union>(thisObject(), other);
}

Object MoreOrEqualThan::complement(const Environment& env)
{
    return makeObject<LessThan>(value);
}

Object MoreOrEqualThan::begin()
{
    return value;
}

MoreOrEqual::MoreOrEqual()
    : Operator(true, 2, true)
{

}

Object MoreOrEqual::operate(const Object& first,
                     const Object& second,
                     Environment& env)
{
    auto f = cast<MoreOrEqualThan>(env, partialApplyRight(second, env));

    return f->apply(first, env);
}

std::string MoreOrEqual::show() const
{
    return MoreOrEqual::defaultName;
}

const std::string MoreOrEqual::defaultName = ">=";

Object MoreOrEqual::partialApplyLeft(const Object& e, Environment& env)
{
     return makeObject<LessThan>(e);
}

Object MoreOrEqual::partialApplyRight(const Object& e, Environment& env)
{
     return makeObject<MoreThan>(e);
}
