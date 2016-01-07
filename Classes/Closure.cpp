#include "Closure.h"
#include <string>
#include "Function.h"
#include "Environment.h"
#include "Pattern.h"
#include "Error.h"
#include "Variable.h"

Closure::Closure(ExpPtrArg body,
                 ExpPtrArg arg,
                 Environment* env,
                 int envSize)
    : body(body),
      argument(arg),
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
    auto variable = d_cast<Variable>(argument);

    if (!variable)
        return make_ptr<ErrorWithMessage>("Incorrect argument definition");

    auto newEnv = this->env->add(variable, e);

    auto evaluated = body->eval(newEnv);

    return evaluated;
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
    return ret + std::string("]") + "(" + argument->show() + "){" + body->show() + "}";
}


// std::string Function::show() const
// {
//     return "(" + argument->show() + "){" + body->show() + "}";
// }
