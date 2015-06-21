#include "Function.h"
#include <iostream>
#include "Pattern.h"
#include "Closure.h"
#include "Environment.h"

namespace Language
{

Function::Function(Expression* body,
                   const std::shared_ptr<Pattern>& argument)
    : body(body),
      argument(argument)
{

}

Function::~Function()
{
    delete body;
}

Expression* Function::apply(Expression* e, Environment*& env)
{
    auto newEnv = env->add(argument,
                           e->evalConstEnv(env));
    return body->evalConstEnv(newEnv);
}

Expression* Function::eval(Environment*& env)
{
    throw std::logic_error("we aren\'t supposed to get here");
    return new Closure(this, env);
}

std::string Function::toString() const
{
    return "\u03BB" + argument->toString() + ".(" + body->toString() + ")";
}

}
