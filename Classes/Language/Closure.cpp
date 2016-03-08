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
}

ExpPtr Closure::apply(ExpPtrArg e, Environment& env) const
{
    auto newEnv = this->env;

    auto fits = this->argument->unapplyVariables(e, newEnv);
    if (!fits)
        return make_ptr<Void>();

    auto evaluated = body->eval(newEnv);

    return Identifier::unwrapIfId(evaluated, newEnv); // costyl
}

std::string Closure::show() const
{
    return "closure[" + argument->show() + " => " + body->show() + "]";
}

bool Closure::operator==(const Expression& other) const
{
    if (typeid(other) == typeid(const Closure))
    {
        auto& x = static_cast<const Closure&>(other);
        return x.body     == body &&
               x.argument == argument &&
               x.env      == env;
    }
    else
        return false;
}
