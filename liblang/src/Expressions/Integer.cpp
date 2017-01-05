#include "Expressions/Integer.h"
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
    else
        return false;
}

std::string Integer::show() const
{
    return std::to_string(value);
}

ExpPtr Integer::next()
{
    return make_ptr<Integer>(value + 1);
}
