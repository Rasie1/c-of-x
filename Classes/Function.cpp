#include "Function.h"
#include <iostream>
#include "Pattern.h"
#include "Closure.h"
#include "Environment.h"

namespace Language
{

Function::Function(const ExpPtr& body,
                   const std::shared_ptr<Pattern>& argument)
    : body(body),
      argument(argument)
{

}

Function::~Function()
{
}

ExpPtr Function::apply(const ExpPtr& e, Environment*& env) const
{
    auto newEnv = env->add(argument,
                           e->evalConstEnv(env));
    return body->evalConstEnv(newEnv);
}

ExpPtr Function::eval(Environment*& env) const
{
    throw  std::logic_error("we aren\'t supposed to get here");
    return std::make_shared<Closure>(
                std::static_pointer_cast<Function>(
                    std::const_pointer_cast<Expression>(shared_from_this())),
                env);
}

std::string Function::toString() const
{
    return "\u03BB" + argument->toString() + ".(" + body->toString() + ")";
}

}
