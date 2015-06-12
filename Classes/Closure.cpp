#include "Closure.h"
#include "Function.h"
#include "Environment.h"
#include "Function.h"

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

Expression* Closure::eval(Environment* env)
{
    return this;
}

Expression* Closure::apply(Expression* e, Environment* env)
{
    return function->apply(e, this->env);
}

std::string Closure::toString() const
{
    return "closure " + function->toString();
}

}
