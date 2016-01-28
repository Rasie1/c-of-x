#include "Integer.h"
#include <string>
#include <algorithm>
#include <iostream>

Integer::Integer(long long value)
    : value(value)
{
}

bool Integer::operator==(const Expression& other) const
{
    if (typeid(other) == typeid(*this))
        return value == static_cast<const Integer&>(other).value;
}

std::string Integer::show() const
{
    return std::to_string(value);
}
