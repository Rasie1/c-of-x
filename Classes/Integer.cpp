#include "Integer.h"
#include <string>
#include <algorithm>
#include "Pattern.h"

namespace Language
{

Integer::Integer(long long value)
    : value(value)
{

}

std::string Integer::toString() const
{
    return std::to_string(value);
}


Pattern* Integer::pattern()
{
    return new PatternAbstraction(toString());
}

}
