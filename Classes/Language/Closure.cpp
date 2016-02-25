#include "Closure.h"
#include <string>
#include "Function.h"
#include "Environment.h"
#include "Error.h"
#include "Identifier.h"
#include "Void.h"

Closure::Closure(ExpPtrArg argument,
                 ExpPtrArg body,
                 const Environment& env,
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

ExpPtr Closure::apply(ExpPtrArg e, Environment& envc) const
{
    auto newEnv = this->env;
    ExpPtr lvalue = argument;
    ExpPtr rvalue = body;
    while (lvalue->unwind(lvalue, rvalue, newEnv));

    auto variable = d_cast<Identifier>(lvalue);
    auto value = d_cast<DataType>(lvalue);

    if (variable)
    {
        newEnv.addEqual(variable, e);
        auto evaluated = rvalue->eval(newEnv);
        if (d_cast<Identifier>(evaluated))
            evaluated = newEnv.getEqual(evaluated);
        return evaluated;
    }
    else if (value)
    {
        if (*value == *e)
        {
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

bool Closure::operator==(const Expression& other) const
{
    throw 0; // todo: implement env eq test
    try
    {
        auto x = dynamic_cast<const Closure&>(other);
        return x.body == body && x.argument == argument;
    }
    catch (std::bad_cast&)
    {
        return false;
    }
}
