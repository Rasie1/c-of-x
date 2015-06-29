#include "PatternWildcard.h"

namespace Language
{

bool PatternWildcard::match(const PatPtr& other,
                            Environment* env) const
{
    return true;
}


std::string PatternWildcard::toString() const
{
    return "_";
}

}
