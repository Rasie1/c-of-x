#include "Function.h"
#include <iostream>
#include "Pattern.h"
#include "Closure.h"
#include "Environment.h"

namespace Language
{

Function::Function(Expression* body, Pattern* argument)
    : body(body),
      argument(argument)
{

}

Function::~Function()
{
    delete body;
    delete argument;
}

Expression* Function::apply(Expression* e, Environment* env)
{
    auto newEnv = env->add(argument,
                           e->eval(env));
    return body->eval(newEnv);
}

Expression* Function::eval(Environment* env)
{
    return new Closure(this, env);
}

std::string Function::toString() const
{
    return "\\" + argument->toString() + ".(" + body->toString() + ")";
}


Expression* PrintPattern::apply(Expression* e, Environment* env)
{
    std::cout << "<" + e->pattern()->toString() + ">";

    return e->eval(env);
}

std::string PrintPattern::toString() const
{
    return "ppat";
}

Expression* Preprocess::apply(Expression* e, Environment* env)
{
    return e->eval(env);
}

std::string Preprocess::toString() const
{
    return "#";
}

}
