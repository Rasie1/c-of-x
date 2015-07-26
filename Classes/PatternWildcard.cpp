#include "PatternWildcard.h"

namespace Language
{

bool PatternWildcard::match(ExpPtrArg other) const
{
    return true;
}


std::string PatternWildcard::toString() const
{
    return "_";
}

}
