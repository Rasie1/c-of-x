#include "Closure.h"
#include <string>
#include "Function.h"
#include "Environment.h"
#include "Error.h"
#include "Identifier.h"
#include "Void.h"

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

    auto variable = d_cast<Identifier>(lvalue);
    auto value = d_cast<DataType>(lvalue);

    if (variable)
    {
        auto newEnv = this->env->addEqual(variable, e);
        auto evaluated = rvalue->eval(newEnv);
        if (d_cast<Identifier>(evaluated))
            evaluated = newEnv->getEqual(evaluated);
        return evaluated;
    }
    else if (value)
    {
        if (*value == *e)
        {
            auto newEnv = env;
            return rvalue->eval(newEnv);
        }
        else
        {
            return make_ptr<Void>();
        }
    }
    else
        return make_ptr<ErrorWithMessage>("Incorrect argument definition");
}

std::string Closure::show() const
{
    return "[|" + argument->show() + " => " + body->show() + "|]";
}
