#include "Closure.h"
#include "Function.h"
#include "Environment.h"
#include "Function.h"
#include "Pattern.h"

namespace Language
{

Closure::Closure(const std::shared_ptr<Function>& function,
                 Environment* env)
    : function(function),
      env(env)
{

}

Closure::~Closure()
{
}

ExpPtr Closure::apply(const ExpPtr& e, Environment*& env) const
{
    return function->applyConstEnv(e->eval(env), this->env);
}

std::string Closure::toString() const
{
    return "closure{"
            + env->top().first->toString()
            + " = "
            + env->top().second->toString()
            + "} "
            + function->toString();
}

}
