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

Object Multiplication::calculate(const Object& l, const Object& r) const
{
    if (auto first = d_cast<Integer>(l))
    if (auto second = d_cast<Integer>(r))
        return makeObject<Integer>(first->value * second->value);
        
    auto operation = makeObject<Operation>(makeObject<Multiplication>(), l, r);
    return makeObject<TypeError>(operation,
                               makeObject<Identifier>("arguments of type integer"),
                               makeObject<Identifier>("arguments: " + l->show() + ", " + r->show()));
}

bool Multiplication::unapplyVariables(const Object& e, const Object& l, const Object& r, Environment &env)
{

    auto lId = checkType<Identifier>(l);
    auto rId = checkType<Identifier>(r);

    if (lId && !rId)
    {
        throw std::logic_error("not implemented");
        // auto value = makeObject<Operation>(makeObject<Division>(), e, r);
        // return l->unapplyVariables(value, env);
    }
    else if (!lId && rId)
    {
        throw std::logic_error("not implemented");
        // auto value = makeObject<Operation>(makeObject<Division>(), e, l);
        // return r->unapplyVariables(value, env);
    }

    auto evaluated = makeObject<Operation>(makeObject<Multiplication>(),
                                         l, r)->eval(env);
    if (auto op = d_cast<Operation>(evaluated))
    {
        if (checkType<Multiplication>(op->op))
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

