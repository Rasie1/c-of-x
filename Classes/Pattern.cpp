#include "Pattern.h"
#include "Application.h"
#include "Integer.h"
#include "Environment.h"

namespace Language
{

Pattern::Pattern()
{

}

bool Pattern::match(Pattern* other, Environment* env)
{
    return true;
}

PatternApplication::PatternApplication(Pattern* body, Pattern* argument)
    : argument(argument),
      body(body)
{

}

bool PatternAbstraction::match(Pattern* other, Environment* env)
{
    auto x = dynamic_cast<PatternAbstraction*>(other);
    if (x == nullptr)
        return false;
    return x->name == name;
}

bool PatternApplication::match(Pattern* other, Environment* env)
{
    auto x = dynamic_cast<PatternApplication*>(other);
    if (x == nullptr)
        return false;
    return body->match(x->body, env);
}

std::string Pattern::toString() const
{
    return "Pattern";
}

std::string PatternAbstraction::toString() const
{
    return name;
}

std::string PatternApplication::toString() const
{
    return "(" + body->toString() + " " + argument->toString() + ")";
}

PatternPredicate::PatternPredicate(Expression* predicate)
    : predicate(predicate)
{

}

bool PatternPredicate::match(Pattern* other, Environment* env)
{
    auto argument = env->get(other);
    auto result  = predicate->apply(argument, env);
    auto integer = dynamic_cast<Integer*>(result);
    if (integer)
        return integer->value != 0;

    return false;
}

std::string PatternPredicate::toString() const
{
    return "(" + predicate->toString() + ") ?";
}

}
