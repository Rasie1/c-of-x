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

std::string Integer::show() const
{
    return std::to_string(value);
}


PatPtr Integer::pattern() const
{
    // Wrong, I think it should return "(== value)?"
    return make_ptr<PatternVariable>(show()); //patternValue
}

}
