#include "Closure.h"
#include "Function.h"
#include "Environment.h"
#include "Function.h"
#include "Pattern.h"
#include <string>

namespace Language
{

Closure::Closure(const std::shared_ptr<Function>& function,
                 Environment* env,
                 int envSize)
    : function(function),
      env(env),
      envSize(envSize)
{

}

Closure::~Closure()
{
    // delete envSize elements
}

ExpPtr Closure::apply(const ExpPtr& e, Environment*& env) const
{
    auto newEnv = env;
    return function->apply(e->eval(newEnv),
                           const_cast<Closure*>(this)->env);
}

std::string Closure::toString() const
{
    std::string ret = "[";
    auto top = env;
    for (int i = 0; i < envSize; ++i)
    {
        ret +=top->top().first->toString()
            + std::string(" = ")
            + top->top().second->toString()
            + std::string(";");
        top = top->getNext();
    }
    return ret + std::string("]") + function->toString();
}

}
