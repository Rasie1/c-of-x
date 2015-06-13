#include "String.h"
#include <string>
#include "PatternAbstraction.h"

namespace Language
{

String::String(const std::string& value)
    : value(value)
{

}

std::string String::toString() const
{
    return "\"" + value + "\"";
}


Pattern* String::pattern()
{
    return new PatternAbstraction(toString());
}

}
