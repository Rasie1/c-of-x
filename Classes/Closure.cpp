#include "Closure.h"
#include "Function.h"
#include "Environment.h"
#include "Function.h"
#include "Pattern.h"

namespace Language
{

Closure::Closure(Function* function, Environment* env)
    : function(function),
      env(env)
{

}

Closure::~Closure()
{
    delete env;
    delete function;
}

Expression* Closure::apply(Expression* e, Environment*& env)
{
    return function->applyConstEnv(e, this->env);
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
