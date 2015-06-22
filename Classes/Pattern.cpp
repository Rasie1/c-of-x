#include "Pattern.h"

namespace Language
{

Pattern::Pattern()
{

}

bool Pattern::match(const PatPtr& other,
                    Environment* env) const
{
    return true;
}

std::string Pattern::toString() const
{
    return "Pattern";
}

}
