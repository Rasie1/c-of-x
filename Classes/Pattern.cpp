#include "Pattern.h"

namespace Language
{

Pattern::Pattern()
{

}

bool Pattern::match(const std::shared_ptr<Pattern>& other,
                    Environment* env) const
{
    return true;
}

std::string Pattern::toString() const
{
    return "Pattern";
}

}
