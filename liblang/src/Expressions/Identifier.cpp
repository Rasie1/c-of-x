#include "Expressions/Identifier.h"
#include "System/Environment.h"
#include <string>
#include <list>
#include "Expressions/Error.h"
#include "Expressions/Any.h"
#include "Expressions/Void.h"
#include "System/Cast.h"

bool Identifier::operator==(const Expression& other) const
{

    if (typeid(*this) == typeid(other))
    {
        auto id = static_cast<const Identifier&>(other);
        return id.name == name;
    }
    else
        return false;
}

std::string Identifier::show() const
{
    return name;
}

bool Identifier::hasFreeVariables(const Environment& envc)
{
    auto env = envc;
    return checkType<Any>(env, env.getEqual(name));
}

Object Identifier::unwrapIfId(const Object& e, const Environment& env)
{
    if (auto id = std::dynamic_pointer_cast<Identifier>(e.expression))
        return const_cast<Environment&>(env).getEqual(id->name);
    else
        return e;
}

Object Identifier::unwrapIfId(const std::shared_ptr<const Expression>& e, const Environment& env)
{
    if (auto id = std::dynamic_pointer_cast<Identifier>(e))
        return const_cast<Environment&>(env).getEqual(id->name);
    else
        return e;
}

bool Identifier::unapplyVariables(const Object& e, Environment& env)
{
    // Just put evaluated e into variable

    auto newEnv = env;
    auto returned = e->eval(newEnv);
    auto value = Identifier::unwrapIfId(returned, newEnv);
    if (checkType<Void>(env, value))
        return false;

    env.addEqual(name, value, true);

    auto intersection = env.getEqual(name);
    if (checkType<Void>(env, intersection))
        return false;

    return true;
}
