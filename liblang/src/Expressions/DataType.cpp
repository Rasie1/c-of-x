#include "Expressions/DataType.h"
#include "System/Environment.h"
#include "Expressions/Void.h"

ExpPtr Data::eval(Environment& env) const
{
    return std::const_pointer_cast<Expression>(shared_from_this());
}

bool Data::unapplyVariables(ExpPtrArg e, Environment& env) const
{
    // why is that?
    return *this == *e;
}


ExpPtr PlainData::intersect(ExpPtrArg other, const Environment& env)
{
    if (*other == *this)
        return other;
    else
        return make_ptr<Void>();
}
