#include "Expressions/DataType.h"
#include "System/Environment.h"
#include "Expressions/Void.h"
#include "Expressions/Not.h"
#include "Expressions/Any.h"
#include "System/Cast.h"

Object Data::eval(Environment& env)
{
    return thisObject();
}

bool Data::unapplyVariables(const Object& e, Environment& env)
{
    if (checkType<Any>(env, e))
        return true;
    return *this == *e;
}


Object PlainData::intersect(const Object& other, Environment& env)
{
    if (*other == *this)
        return other;
    else
        return makeObject<Void>();
}
