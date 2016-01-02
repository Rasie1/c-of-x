#include "String.h"
#include <string>
#include "PatternVariable.h"

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
