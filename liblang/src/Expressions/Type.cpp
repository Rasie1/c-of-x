#include "Expressions/Type.h"

std::string Type::show() const
{
    return defaultName;
}

bool Type::operator==(const Expression& other) const
{
    if (typeid(*this) == typeid(other))
        return true;
        
    return false;
}


const std::string Type::defaultName = "type";
