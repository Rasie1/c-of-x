#include "Expressions/Integer.h"
#include <string>
#include <algorithm>
#include <iostream>
#include "System/Cast.h"
#include "Expressions/IntegerType.h"
#include "Expressions/Equality.h"

Integer::Integer(long long value) :
    value(value)
{
}

Object Integer::equals(const Object& other, Environment& env)
{
    if (auto i = cast<Integer>(env, other))
    {
        if (*this == *i)
            return thisObject();
        else
            return makeObject<Void>();
    }
    return makeOperation<Equality>(thisObject(), other);
}

bool Integer::operator==(const Expression& other) const
{
    if (typeid(other) == typeid(*this))
        return value == static_cast<const Integer&>(other).value;
    else
        return false;
}

std::string Integer::show() const
{
    return std::to_string(value);
}

Object Integer::next()
{
    return makeObject<Integer>(value + 1);
}
