#include "Expressions/Inverse.h"
#include "Expressions/Void.h"
#include "Expressions/Identifier.h"
#include "System/Cast.h"

Object Inverse::apply(const Object& e, Environment& env)
{
    auto value = Identifier::unwrapIfId(e, env);
    if (auto f = cast<Morphism>(env, value))
        return f->inverse();
    else
        return makeObject<Void>();
}

std::string Inverse::show() const
{
    return defaultName;
}

const std::string Inverse::defaultName = "inverse";

