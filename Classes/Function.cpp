#include "Function.h"
#include <iostream>
#include "Pattern.h"
#include "Closure.h"
#include "Environment.h"

namespace Language
{

Function::Function(const ExpPtr& body,
                   const PatPtr& argument)
    : body(body),
      argument(argument)
{
}

Function::~Function()
{
}

ExpPtr Function::apply(const ExpPtr& e, Environment*& env) const
{
    auto newEnv1 = env;
    auto newEnv2 = env->add(argument,
                            e->eval(newEnv1));
    return body->eval(newEnv2);
}

ExpPtr Function::evaluation(Environment*& env) const
{
    throw  std::logic_error("we aren\'t supposed to get here");
    return std::make_shared<Closure>(
                std::static_pointer_cast<Function>(
                    std::const_pointer_cast<Expression>(shared_from_this())),
                env);
}

std::string Function::toString() const
{
    return "(" + argument->toString() + ")._(" + body->toString() + ")";
}

}
