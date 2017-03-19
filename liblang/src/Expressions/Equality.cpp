#include "Expressions/Equality.h"
#include "Expressions/Identifier.h"
#include "System/Environment.h"
#include "Expressions/Operation.h"
#include "Expressions/Intersection.h"
#include "Expressions/Void.h"
#include "Expressions/Any.h"
#include "Expressions/Lambda.h"
#include "Expressions/Application.h"
#include "System/Cast.h"


optional<bool> Equals::holds(const Object& e, const Environment& env)
{
    if (*value == *e)
        return make_optional(true);

    auto operation = make_ptr<Operation>(make_ptr<Intersection>(), value, e);
    auto intersection = operation->eval(const_cast<Environment&>(env));

    if (*intersection == *value || *intersection == *e)
        return make_optional(true);

    return none;
}

bool Equals::operator==(const Expression& other) const
{
    if (typeid(*this) == typeid(other))
    {
        auto x = static_cast<const Equals&>(other);
        return value == x.value;
    }
    else
        return false;
}

std::string Equals::show() const
{
    return defaultName + "(" + value->show() + ")";
}

const std::string Equals::defaultName = "=";

Object Equals::intersect(const Object& other, const Environment& env)
{
    if (checkType<Any>(value))
        return other;
    if (auto equals = d_cast<Equals>(other))
    {
        auto l = value;
        auto r = equals->value;

        return make_ptr<Equals>(l->intersect(r, env));
    }
    return make_ptr<Operation>(make_ptr<Intersection>(), shared_from_this(), other);
}

Object Equals::element(const Environment& env)
{
    return value;
}

Equality::Equality()
    : Operator(true, 1, true)
{

}

static bool operateHelper(const Object& first,
                          const Object& second,
                          Environment& env)
{
    auto lvalue = first;//Identifier::unwrapIfId(first, env);
    auto rvalue = second;

    auto ret = lvalue->unapplyVariables(rvalue, env);

    env.debugPrint("unapplied: (" + lvalue->show() + ") = (" + rvalue->show() + ")\n", true);

    return ret;
}

Object Equality::operate(const Object& first,
                         const Object& second,
                         Environment& env)
{
    env.debugPrint("Equality: (" + first->show() + ") = (" + second->show() + ")\n", true);
    env.debugPrint("L R:\n", true);
    env.increaseDebugIndentation();
    operateHelper(first, second, env);
    env.decreaseDebugIndentation();

    Object ret = first;

    return ret;
}

std::string Equality::show() const
{
    return Equality::defaultName;
}

const std::string Equality::defaultName = "=";

Object Equality::partialApplyLeft(const Object& e, Environment& env)
{
     return make_ptr<Equals>(e);
}

Object Equality::partialApplyRight(const Object& e, Environment& env)
{
     return make_ptr<Equals>(e);
}
