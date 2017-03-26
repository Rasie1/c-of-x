#include "Expressions/Multiplication.h"
#include "Expressions/Operation.h"
#include "Expressions/Integer.h"
#include "Expressions/Subtraction.h"
#include "Expressions/Identifier.h"
#include "Expressions/TypeError.h"
#include "System/Environment.h"
#include "Expressions/Any.h"
#include "System/Cast.h"

Multiplication::Multiplication()
    : CalculationOperator(false, 7, true)
{

}

Object Multiplication::calculate(const Object& l, 
                                 const Object& r,
                                 Environment& env) const
{
    if (auto first = cast<Integer>(env, l))
    if (auto second = cast<Integer>(env, r))
        return makeObject<Integer>(first->value * second->value);
        
    auto operation = makeOperation<Multiplication>(l, r);
    return makeObject<TypeError>(operation,
                               makeObject<Identifier>("arguments of type integer"),
                               makeObject<Identifier>("arguments: " + l->show() + ", " + r->show()));
}

bool Multiplication::unapplyVariables(const Object& e, const Object& l, const Object& r, Environment &env)
{

    auto lId = checkType<Identifier>(env, l);
    auto rId = checkType<Identifier>(env, r);

    if (lId && !rId)
    {
        throw std::logic_error("not implemented");
        // auto value = makeOperation<Division>(e, r);
        // return l->unapplyVariables(value, env);
    }
    else if (!lId && rId)
    {
        throw std::logic_error("not implemented");
        // auto value = makeOperation<Division>(e, l);
        // return r->unapplyVariables(value, env);
    }

    auto evaluated = makeOperation<Multiplication>(l, r)->eval(env);
    if (auto op = cast<Operation>(env, evaluated))
    {
        if (checkType<Multiplication>(env, op->op))
            return op->left->unapplyVariables(l, env)
                && op->right->unapplyVariables(r, env);
    }
    return evaluated->unapplyVariables(e, env);
}

std::string Multiplication::show() const
{
    return Multiplication::defaultName;
}

const std::string Multiplication::defaultName = "*";

