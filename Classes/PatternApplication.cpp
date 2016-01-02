#include "PatternApplication.h"
#include "Application.h"
#include "Environment.h"

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

std::string PatternApplication::show() const
{
    return "(" + body->show() + " " + argument->show() + ")";
}

