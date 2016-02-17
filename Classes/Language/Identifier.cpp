#include "Identifier.h"
#include "Environment.h"
#include <string>
#include <list>
#include "Error.h"
#include "Any.h"

bool Identifier::operator==(const Expression& other) const
{
    try
    {
        auto id = dynamic_cast<const Identifier&>(other);
        return id.name == name;
    }
    catch (std::bad_cast& bc)
    {
        return false;
    }
}

std::string Identifier::show() const
{
    return name;
}

bool Identifier::hasNonOpVariable(Environment* env) const
{
    return d_cast<Any>(env->getEqual(std::const_pointer_cast<Expression>(shared_from_this()))) != nullptr;
}

ExpPtr Identifier::unwrapIfId(ExpPtrArg e, Environment* env)
{
    if (d_cast<Identifier>(e))
        return env->getEqual(e);
    else
        return e;
}
