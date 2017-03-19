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

Object Subtraction::calculate(const Object& l, const Object& r) const
{
    auto firstInteger  = d_cast<Integer>(l);
    auto secondInteger = d_cast<Integer>(r);

    if (!firstInteger || !secondInteger)
    {
        auto operation = makeObject<Operation>(makeObject<Subtraction>(), l, r);
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
    auto lId = checkType<Identifier>(l);
    auto rId = checkType<Identifier>(r);

    if (lId && !rId)
    {
        auto value = makeObject<Operation>(makeObject<Addition>(), e, r);
        return l->unapplyVariables(value, env);
    }
    else if (!lId && rId)
    {
        auto value = makeObject<Operation>(makeObject<Addition>(), e, l);
        return r->unapplyVariables(value, env);
    }

    auto evaluated = makeObject<Operation>(makeObject<Subtraction>(),
                                         l, r)->eval(env);

    if (auto op = d_cast<Operation>(evaluated))
    {
        if (checkType<Subtraction>(op->op))
            return op->left->unapplyVariables(l, env)
                && op->right->unapplyVariables(r, env);
    }
    return evaluated->unapplyVariables(e, env);
}
