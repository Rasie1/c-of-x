#include "Any.h"

std::string Any::show() const
{
    return defaultName;
}

const std::string Any::defaultName = "_";
