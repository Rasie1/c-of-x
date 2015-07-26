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

bool PatternApplication::match(ExpPtrArg other,
                               Environment* env) const
{
    auto x = std::dynamic_pointer_cast<PatternApplication>(other);
    if (x)
        return false;
    return body->match(x->body, env);
}

std::string PatternApplication::toString() const
{
    return "(" + body->toString() + " " + argument->toString() + ")";
}

}
