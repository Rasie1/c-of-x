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
#include "System/Cast.h"

Closure::Closure(const Object& argument,
                 const Object& body,
                 const Environment& env)
    : body(body),
      argument(argument),
      env(env)
{

}

Closure::~Closure()
{
}

static bool isExpressionQuoted(const Object& e, Environment& env)
{
    if (checkType<QuotedExpression>(e))
        return true;
    if (auto op = cast<Operation>(e))
    {
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

Object Closure::apply(const Object& e, Environment& env)
{
    // In case of closure as predicate, it is more likely that closure's body intersects with only
    // argument subset, leaving arguments' complement in context of applied value as it was before

    auto value = Identifier::unwrapIfId(e, env);
    auto newEnv = this->env;

    auto fits = this->argument->unapplyVariables(value, newEnv);
    if (!fits)
    {
        return makeObject<Void>();
    }

    auto evaluated = body->eval(newEnv);

    if (checkType<Identifier>(evaluated) && 
        *evaluated == *this->argument)
    {
        auto id = cast<Identifier>(evaluated);
        auto value = Identifier::unwrapIfId(evaluated, newEnv);
        env.addEqual(id->name, value);
        return e;
    }
    else if (checkType<Operation>(evaluated))
    {
        return makeOperation<Application>(thisObject(), e);
    }

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

Object Closure::codomain()
{
    // return makeObject<Equals>(body);
    // TODO: not only body. Constraints from argument should be also applied here.
    // TODO: environment counts as codomain as well

    return makeObject<Equals>(makeOperation<Application>(thisObject(),
                                                         makeObject<Any>()));
}

Object Closure::domain()
{
    return makeObject<Equals>(argument);
    // TODO: not only argument. Constraints from body should be also applied here. Probably.
}

bool Closure::unapplyVariables(const Object& e, Environment& env)
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

Object Closure::inverse()
{
    return makeObject<Closure>(body, argument, env);
}
