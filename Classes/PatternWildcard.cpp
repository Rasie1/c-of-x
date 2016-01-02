#include "PatternWildcard.h"

bool PatternWildcard::match(ExpPtrArg other) const
{
    return true;
}


std::string PatternWildcard::show() const
{
    return "_";
}

