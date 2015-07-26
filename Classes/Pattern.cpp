#include "Pattern.h"

namespace Language
{

Pattern::Pattern()
{

}

bool Pattern::match(ExpPtrArg other) const
{
    return false;
}

PatPtr Pattern::pattern() const
{
    return Expression::pattern();
    //auto e = std:: const_pointer_cast<Expression>(shared_from_this());
    //auto p = s_cast<Pattern>(e);
    //return p;
}


std::string Pattern::toString() const
{
    return "unknownpattern";
}


ExpPtr ReturnPattern::apply(ExpPtrArg e, Environment*& env) const
{
    return e->pattern();
}

std::string ReturnPattern::toString() const
{
    return defaultName;
}

const std::string ReturnPattern::defaultName = "pattern";

}
