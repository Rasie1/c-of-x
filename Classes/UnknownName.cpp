#include "UnknownName.h"
#include "PatternVariable.h"

namespace Language
{

PatPtr UnknownName::pattern() const
{
    return make_ptr<PatternVariable>(name);
}

std::string UnknownName::toString() const
{
    return "UnknownName(" + name + ")";
}

}
