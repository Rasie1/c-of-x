#include "Expressions/Not.h"
#include "Expressions/Application.h"
#include "Expressions/Operation.h"
#include "Expressions/Identifier.h"
#include "Expressions/Void.h"
#include "System/Environment.h"
#include "System/Cast.h"

Object Not::apply(const Object& e, Environment& env)
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

bool Complement::unapplyVariables(const Object& e, Environment& env)
{
    auto newEnv = env;
    auto result = this->e->unapplyVariables(e, newEnv);
    return !result;
}

Object Complement::intersect(const Object& other, const Environment& env)
{
    if (*e == *other)
        return makeObject<Void>();
    else
        return thisObject();
}

Object Complement::complement(const Environment& env)
{
    return e;
}

Object Complement::eval(Environment& env)
{
    return e->complement(env);
}
