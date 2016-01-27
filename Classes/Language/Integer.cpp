#include "Integer.h"
#include <string>
#include <algorithm>
#include <iostream>

Integer::Integer(long long value)
    : value(value)
{
}

bool Integer::match(ExpPtrArg other) const
{
    auto integer = d_cast<Integer>(other);
    if (!integer)
        return false;

    return this->value == integer->value;
}

std::string Integer::show() const
{
    return std::to_string(value);
}
