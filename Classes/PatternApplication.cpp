#include "PatternApplication.h"
#include "Application.h"
#include "Environment.h"

namespace Language
{

PatternApplication::PatternApplication(Pattern* body, Pattern* argument)
    : argument(argument),
      body(body)
{

}

bool PatternApplication::match(Pattern* other, Environment* env)
{
    auto x = dynamic_cast<PatternApplication*>(other);
    if (x == nullptr)
        return false;
    return body->match(x->body, env);
}

std::string PatternApplication::toString() const
{
    return "(" + body->toString() + " " + argument->toString() + ")";
}

}
