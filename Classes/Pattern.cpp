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
    return "_";
}


ExpPtr ReturnPattern::apply(const ExpPtr& e, Environment*& env) const
{
    return e->pattern();
}

std::string ReturnPattern::toString() const
{
    return defaultName;
}

const std::string ReturnPattern::defaultName = "pattern";

}
