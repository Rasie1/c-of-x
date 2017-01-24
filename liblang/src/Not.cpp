#include "Expressions/Not.h"
#include "Expressions/Application.h"
#include "Expressions/Operation.h"
#include "Expressions/Identifier.h"
#include "Expressions/Void.h"
#include "System/Environment.h"

ExpPtr Not::apply(ExpPtrArg e, Environment& env) const
{
    return e->complement(env);
}

std::string Not::show() const
{
    return defaultName;
}

const std::string Not::defaultName = "!";


std::string Complement::show() const
{
    return defaultName + "(" + e->show() + ")";
}

const std::string Complement::defaultName = "!";

bool Complement::unapplyVariables(ExpPtrArg e, Environment& env) const
{
    auto newEnv = env;
    auto result = this->e->unapplyVariables(e, newEnv);
    return !result;
}

ExpPtr Complement::intersect(ExpPtrArg other, const Environment& env)
{
    if (*e == *other)
        return make_ptr<Void>();
    else
        return shared_from_this();
}

ExpPtr Complement::complement(const Environment& env)
{
    return e;
}
