#include "Expressions/Closure.h"
#include <string>
#include "Expressions/Morphism.h"
#include "System/Environment.h"
#include "Expressions/Error.h"
#include "Expressions/Equality.h"
#include "Expressions/Identifier.h"
#include "Expressions/Void.h"
#include "Expressions/Any.h"
#include "Expressions/Quote.h"
#include "Expressions/Operation.h"
#include "Expressions/Application.h"

Closure::Closure(ExpPtrArg argument,
                 ExpPtrArg body,
                 const Environment& env)
    : body(body),
      argument(argument),
      env(env)
{

}

Closure::~Closure()
{
}


static bool isExpressionQuoted(ExpPtrArg e, Environment& env)
{
    if (checkType<QuotedExpression>(e))
        return true;
    if (checkType<Operation>(e))
    {
        auto op = s_cast<Operation>(e);
        if (checkType<Application>(op->op))
        {
            if (checkType<Quote>(Identifier::unwrapIfId(op->left, env))) // really?
            {
                return true;
            }
        }
    }
    return false;
}


ExpPtr Closure::apply(ExpPtrArg e, Environment& env) const
{
    auto value = Identifier::unwrapIfId(e, env);
    auto newEnv = this->env;

    auto fits = this->argument->unapplyVariables(value, newEnv);
    if (!fits)
    {
        return make_ptr<Void>();
    }

    auto evaluated = body->eval(newEnv);

//    if (isExpressionQuoted(argument, env))
//    {
//        env = newEnv; // not correct, it should affect only bound variables
//        return evaluated;
//    }
//    else
        return Identifier::unwrapIfId(evaluated, newEnv);
}

std::string Closure::show() const
{
    return "closure[" + argument->show() + " -> " + body->show() + "]";
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

ExpPtr Closure::codomain()
{
    // return make_ptr<Equals>(body);
    // TODO: not only body. Constraints from argument should be also applied here.
    // TODO: environment counts as codomain as well

    return make_ptr<Equals>(
               make_ptr<Operation>(make_ptr<Application>(),
                                   shared_from_this(),
                                   make_ptr<Any>()));
}

ExpPtr Closure::domain()
{
    return make_ptr<Equals>(argument);
    // TODO: not only argument. Constraints from body should be also applied here. Probably.
}

bool Closure::unapplyVariables(ExpPtrArg e, Environment& env) const
{
    // this is somewhat opposite of case 
    // f x = y ---> f = x -> y

    // maybe like this
    // x -> y = f
    // y = f x

    // or this
    // (x -> y) a = b
    // (x -> y) = a -> b
    // y = (a -> b) x
    
    auto unapplied = body->unapplyVariables(e, env);

    return unapplied;
}

ExpPtr Closure::inverse() const
{
    throw std::logic_error("unimplemented");

    // TODO: calculate inverse function out of arg and body
}
