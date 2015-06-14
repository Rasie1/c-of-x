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

Expression* Function::applyConstEnv(Expression* e, Environment* env)
{
    auto newEnv = env->add(argument,
                           e->evalConstEnv(env));
    return body->evalConstEnv(newEnv);
}

Expression* Function::evalConstEnv(Environment* env)
{
    return new Closure(this, env);
}

std::string Function::toString() const
{
    return "\\" + argument->toString() + ".(" + body->toString() + ")";
}


Expression* PrintPattern::applyConstEnv(Expression* e, Environment* env)
{
    std::cout << e->pattern()->toString();

    return e->evalConstEnv(env);
}

std::string PrintPattern::toString() const
{
    return defaultName;
}

const std::string PrintPattern::defaultName = "printPattern";

Expression* Preprocess::applyConstEnv(Expression* e, Environment* env)
{
    return e->evalConstEnv(env);
}

std::string Preprocess::toString() const
{
    return "#";
}

}
