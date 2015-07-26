#include "PatternApplication.h"
#include "Application.h"
#include "Environment.h"

namespace Language
{

PatternApplication::PatternApplication(PatPtrArg body,
                                       PatPtrArg argument)
    : argument(argument),
      body(body)
{

}

bool PatternApplication::match(ExpPtrArg other) const
{
    auto x = d_cast<PatternApplication>(other);
    if (x)
        return false;
    return body->match(x->body);
}

std::string PatternApplication::toString() const
{
    return "(" + body->toString() + " " + argument->toString() + ")";
}

}
