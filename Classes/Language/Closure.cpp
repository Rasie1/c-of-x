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
    : body(body),//Identifier::unwrapIfId(body, env)),
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
    newEnv.erase(lvalue);
;
    if (typeid(*lvalue) == typeid(Identifier))
    {
        newEnv.addEqual(lvalue, Identifier::unwrapIfId(e, envc));
        auto evaluated = rvalue->eval(newEnv);
        evaluated = Identifier::unwrapIfId(evaluated, newEnv);

        return evaluated;
    }
    else if (std::shared_ptr<DataType> lvalue = d_cast<DataType>(lvalue))
    {
        if (*lvalue == *e)
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
    if (typeid(other) == typeid(const Closure))
    {
        auto& x = static_cast<const Closure&>(other);
        return x.body == body &&
               x.argument == argument &&
               x.env == env;
    }
    else
        return false;
}
