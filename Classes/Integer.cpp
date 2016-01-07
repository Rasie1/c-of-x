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
    return true;
}

std::string Integer::show() const
{
    return std::to_string(value);
}
