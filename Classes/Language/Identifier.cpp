#include "Identifier.h"
#include "Environment.h"
#include <string>
#include <list>
#include "Error.h"
#include "Any.h"

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
    return "\'" + name;
}

bool Identifier::hasFreeVariables(const Environment& env) const
{
    return d_cast<Any>(env.getEqual(std::const_pointer_cast<Expression>(shared_from_this()))) != nullptr;
}

ExpPtr Identifier::unwrapIfId(ExpPtrArg e, const Environment& env)
{
    if (d_cast<Identifier>(e))
        return env.getEqual(e);
    else
        return e;
}

void Identifier::unapplyVariables(ExpPtrArg e, Environment& env) const
{
    env.addEqual(shared_from_this(), e);
}
