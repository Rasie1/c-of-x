#include "Integer.h"
#include <string>
#include <algorithm>
#include "PatternAbstraction.h"

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
    // Wrong, I think it should return "(== value)?"
    return new PatternAbstraction(toString());
}

}
