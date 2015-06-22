#include "UnknownName.h"
#include "PatternVariable.h"

namespace Language
{

PatPtr UnknownName::pattern() const
{
    return std::make_shared<PatternVariable>(name);
}

std::string UnknownName::toString() const
{
    return "UnknownName(" + name + ")";
}

}
