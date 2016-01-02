#include "UnknownName.h"
#include "PatternVariable.h"

PatPtr UnknownName::pattern() const
{
    return make_ptr<PatternVariable>(name);
}

std::string UnknownName::show() const
{
    return "UnknownName(" + name + ")";
}
