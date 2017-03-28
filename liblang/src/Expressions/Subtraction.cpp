#include "Expressions/Subtraction.h"
#include <string>
#include <stdexcept>
#include "Expressions/Integer.h"
#include "Expressions/Operation.h"
#include "Expressions/Addition.h"
#include "Expressions/Identifier.h"
#include "Expressions/TypeError.h"
#include "System/Environment.h"
#include "System/Cast.h"

Subtraction::Subtraction()
    : CalculationOperator(false, 5, true)
{
}

Object Subtraction::calculate(const Object& l, 
                              const Object& r,
                              Environment& env) const
{
    auto firstInteger  = cast<Integer>(env, l);
    auto secondInteger = cast<Integer>(env, r);

    if (!firstInteger || !secondInteger)
    {
        auto operation = makeOperation<Subtraction>(l, r);
        return makeObject<TypeError>(operation,
                                   makeObject<Identifier>("int"),
                                   makeObject<Identifier>("?"));
    }

    return makeObject<Integer>(firstInteger->value - secondInteger->value);
}

std::string Subtraction::show() const
{
    return Subtraction::defaultName;
}

const std::string Subtraction::defaultName = "-";
bool Subtraction::unapplyVariables(const Object& e, const Object& l, const Object& r, Environment &env)
{
    auto lId = checkType<Identifier>(env, l);
    auto rId = checkType<Identifier>(env, r);

    if (lId && !rId)
    {
        auto value = makeOperation<Addition>(e, r);
        return l->unapplyVariables(value, env);
    }
    else if (!lId && rId)
    {
        auto value = makeOperation<Addition>(e, l);
        return r->unapplyVariables(value, env);
    }

    auto evaluated = makeOperation<Subtraction>(l, r);

    if (auto op = cast<Operation>(env, evaluated))
    {
        if (checkType<Subtraction>(env, op->op))
            return op->left->unapplyVariables(l, env)
                && op->right->unapplyVariables(r, env);
    }
    return evaluated->unapplyVariables(e, env);
}
