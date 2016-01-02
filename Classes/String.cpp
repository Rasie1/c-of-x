#include "String.h"
#include <string>
#include "PatternVariable.h"

namespace Language
{

String::String(const std::string& value)
    : value(value)
{

}

std::string String::show() const
{
    return "\"" + value + "\"";
}


PatPtr String::pattern() const
{
    return make_ptr<PatternVariable>(show());
}

}
