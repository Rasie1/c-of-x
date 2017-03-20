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

bool Identifier::hasFreeVariables(const Environment& env)
{
    return checkType<Any>(env.getEqual(thisObject()));
}

Object Identifier::unwrapIfId(const Object& e, const Environment& env)
{
    if (cast<Identifier>(e))
        return env.getEqual(e);
    else
        return e;
}

bool Identifier::unapplyVariables(const Object& e, Environment& env)
{
    // Just put evaluated e into variable

    auto newEnv = env;
    auto returned = e->eval(newEnv);
    auto value = Identifier::unwrapIfId(returned, newEnv);
    if (checkType<Void>(value))
        return false;

    env.addEqual(thisObject(), value, true);

    auto intersection = env.getEqual(thisObject());
    if (checkType<Void>(intersection))
        return false;

    return true;
}
