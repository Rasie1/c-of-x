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


optional<bool> Equals::holds(const Object& e, Environment& envc)
{
    auto env = envc;
    if (auto eq = value->equals(e, env))
    {
        if (checkType<Void>(env, eq))
            return make_optional(false);
        else
            return make_optional(true);
    }

    // auto operation = makeOperation<Intersection>(value, e);
    // auto intersection = operation->eval(const_cast<Environment&>(env));

    // if (*intersection == *value || *intersection == *e)
    //     return make_optional(true);

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

Object Equals::intersect(const Object& other, Environment& envc)
{
    auto env = envc;
    if (checkType<Any>(env, value))
        return other;
    if (auto equals = cast<Equals>(env, other))
    {
        auto l = value;
        auto r = equals->value;

        return makeObject<Equals>(l->intersect(r, env));
    }
    return makeOperation<Intersection>(thisObject(), other);
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
     return makeObject<Equals>(e);
}

Object Equality::partialApplyRight(const Object& e, Environment& env)
{
     return makeObject<Equals>(e);
}
