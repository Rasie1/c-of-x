#include "Expressions/DataType.h"
#include "System/Environment.h"
#include "Expressions/Void.h"
#include "Expressions/Not.h"
#include "Expressions/Any.h"
#include "System/Cast.h"

Object Data::eval(Environment& env) const
{
    return std::const_pointer_cast<Expression>(shared_from_this());
}

bool Data::unapplyVariables(const Object& e, Environment& env)
{
    if (checkType<Any>(e))
        return true;
    return *this == *e;
}


Object PlainData::intersect(const Object& other, const Environment& env)
{
    if (*other == *this)
        return other;
    else
        return makeObject<Void>();
}
