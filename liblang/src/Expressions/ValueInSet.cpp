#include "Expressions/ValueInSet.h"
#include "Expressions/Equality.h"

std::string ValueInSet::show() const
{
    return "(\u0251 : " + this->set->show() + ")";
}

bool ValueInSet::unapplyVariables(ExpPtrArg e, Environment& env) const
{
    // TODO: something more
    return *this == *e;
}

ExpPtr ValueInSet::eval(Environment& env) const
{
    return set->takeValue(env);
}

