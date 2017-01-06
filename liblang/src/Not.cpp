#include "Expressions/Not.h"
#include "Expressions/Application.h"
#include "Expressions/Operation.h"
#include "Expressions/Identifier.h"

ExpPtr Not::apply(ExpPtrArg e, Environment& env) const
{
    return make_ptr<Complement>(e);
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
    // TODO: implement
    return false;
}
