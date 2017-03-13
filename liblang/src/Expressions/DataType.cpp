#include "Expressions/DataType.h"
#include "System/Environment.h"
#include "Expressions/Void.h"
#include "Expressions/Not.h"
#include "Expressions/Any.h"

ExpPtr Data::eval(Environment& env) const
{
    return std::const_pointer_cast<Expression>(shared_from_this());
}

bool Data::unapplyVariables(ExpPtrArg e, Environment& env)
{
    if (checkType<Any>(e))
        return true;
    return *this == *e;
}


ExpPtr PlainData::intersect(ExpPtrArg other, const Environment& env)
{
    if (*other == *this)
        return other;
    else
        return make_ptr<Void>();
}
