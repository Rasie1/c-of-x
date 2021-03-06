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

Object Complement::intersect(const Object& other, Environment& env)
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
    auto ret = e->complement(env);

    if (auto complement = castNoEval<Complement>(ret))
    if (*complement->e == *this->e)
    {
        env.debugPrint("identity restored\n");
        ret = thisObject();
    }

    return ret;
}

bool Complement::operator==(const Expression& other) const
{
    return typeid(*this) == (typeid(other));
    try
    {
        auto x = dynamic_cast<const Complement&>(other);
        return x.e == this->e;
    }
    catch (std::bad_cast&)
    {
        return false;
    }
}
