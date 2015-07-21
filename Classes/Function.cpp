#include "Function.h"
#include <iostream>
#include "Pattern.h"
#include "Closure.h"
#include "Environment.h"
#include "String.h"

namespace Language
{

Function::Function(const ExpPtr& body,
                   const ExpPtr& argument)
    : body(body),
      argument(argument)
{
}

Function::~Function()
{
}

ExpPtr Function::apply(const ExpPtr& e, Environment*& env) const
{
    /*auto newEnv1 = env;
    auto newEnv2 = env->add(argument,
                            e->eval(newEnv1));*/
    //if (!argument->match(e, env))
    //    return std::make_shared<String>("!No Match!");

    env = env->add(argument, e);
    return body->eval(env);
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
    return "(" + argument->toString() + "){" + body->toString() + "}";
}

}
