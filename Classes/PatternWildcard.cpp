#include "PatternWildcard.h"

namespace Language
{

bool PatternWildcard::match(ExpPtrArg other,
                            Environment* env) const
{
    return true;
}


std::string PatternWildcard::toString() const
{
    return "_";
}

}
