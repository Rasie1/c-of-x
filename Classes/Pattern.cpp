#include "Pattern.h"

namespace Language
{

Pattern::Pattern()
{

}

bool Pattern::match(Pattern* other, Environment* env)
{
    return true;
}

std::string Pattern::toString() const
{
    return "Pattern";
}

}
