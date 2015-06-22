#include "String.h"
#include <string>
#include "PatternVariable.h"

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


PatPtr String::pattern() const
{
    return std::make_shared<PatternVariable>(toString());
}

}
