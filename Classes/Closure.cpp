#include "Closure.h"
#include <string>
#include "Function.h"
#include "Environment.h"
#include "Error.h"
#include "Variable.h"

Closure::Closure(ExpPtrArg argument,
                 ExpPtrArg body,
                 Environment* env,
                 int envSize)
    : body(body),
      argument(argument),
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
    ExpPtr lvalue = argument;
    ExpPtr rvalue = body;
    while (lvalue->unwind(lvalue, rvalue, env));

    auto variable = d_cast<Variable>(lvalue);
    auto value = d_cast<DataType>(argument);

    if (variable)
    {
        auto newEnv = this->env->add(variable, e);
        auto evaluated = rvalue->eval(newEnv);
        return evaluated;
    }
    else if (value)
    {
        if (value->match(e))
        {
            auto newEnv = env;
            return rvalue->eval(newEnv);
        }
        else
        {
            return make_ptr<ErrorWithMessage>("No Match");
        }
    }
    else
        return make_ptr<ErrorWithMessage>("Incorrect argument definition");
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
    ret += std::string("]");
    return "(" + argument->show() + "){" + body->show() + "}";
}
