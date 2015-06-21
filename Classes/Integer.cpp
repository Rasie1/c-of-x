#include "Integer.h"
#include <string>
#include <algorithm>
#include "PatternVariable.h"

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


std::shared_ptr<Pattern> Integer::pattern() const
{
    // Wrong, I think it should return "(== value)?"
    return std::make_shared<PatternVariable>(toString());
}

}
