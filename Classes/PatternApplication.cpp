#include "PatternApplication.h"
#include "Application.h"
#include "Environment.h"

namespace Language
{

PatternApplication::PatternApplication(const std::shared_ptr<Pattern>& body,
                                       const std::shared_ptr<Pattern>& argument)
    : argument(argument),
      body(body)
{

}

bool PatternApplication::match(const std::shared_ptr<Pattern>& other,
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
