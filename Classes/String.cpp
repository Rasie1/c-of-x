#include "String.h"
#include <string>

String::String(const std::string& value)
    : value(value)
{

}

std::string String::show() const
{
    return "\"" + value + "\"";
}
