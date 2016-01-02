#include "Closure.h"
#include "Function.h"
#include "Environment.h"
#include "Function.h"
#include "Pattern.h"
#include <string>

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

ExpPtr Closure::apply(ExpPtrArg e, Environment*& env) const
{
    auto newEnv = env;
    return function->apply(e->eval(newEnv),
                           const_cast<Closure*>(this)->env);
}

std::string Closure::show() const
{
    std::string ret = "[";
    auto top = env;
    for (int i = 0; i < envSize; ++i)
    {
        ret +=top->top().first->show()
            + std::string(" = ")
            + top->top().second->show()
            + std::string(";");
        top = top->getNext();
    }
    return ret + std::string("]") + function->show();
}
