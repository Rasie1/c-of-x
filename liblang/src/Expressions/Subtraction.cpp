#include "Expressions/Subtraction.h"
#include <string>
#include <stdexcept>
#include "Expressions/Integer.h"
#include "Expressions/Operation.h"
#include "Expressions/Addition.h"
#include "Expressions/Identifier.h"
#include "Expressions/TypeError.h"
#include "System/Environment.h"

Subtraction::Subtraction()
    : CalculationOperator(false, 5, true)
{
}

ExpPtr Subtraction::calculate(ExpPtrArg l, ExpPtrArg r) const
{
    auto firstInteger  = d_cast<Integer>(l);
    auto secondInteger = d_cast<Integer>(r);

    if (!firstInteger || !secondInteger)
    {
        auto operation = make_ptr<Operation>(make_ptr<Subtraction>(), l, r);
        return make_ptr<TypeError>(operation,
                                   make_ptr<Identifier>("int"),
                                   make_ptr<Identifier>("?"));
    }

    return make_ptr<Integer>(firstInteger->value - secondInteger->value);
}

std::string Subtraction::show() const
{
    return Subtraction::defaultName;
}

const std::string Subtraction::defaultName = "-";
bool Subtraction::unapplyVariables(ExpPtrArg e, ExpPtrArg l, ExpPtrArg r, Environment &env) const
{
    auto lId = checkType<Identifier>(l);
    auto rId = checkType<Identifier>(r);

    if (lId && !rId)
    {
        auto value = make_ptr<Operation>(make_ptr<Addition>(), e, r);
        return l->unapplyVariables(value, env);
    }
    else if (!lId && rId)
    {
        auto value = make_ptr<Operation>(make_ptr<Addition>(), e, l);
        return r->unapplyVariables(value, env);
    }

    auto evaluated = make_ptr<Operation>(make_ptr<Subtraction>(),
                                         l, r)->eval(env);

    if (auto op = d_cast<Operation>(evaluated))
    {
        if (checkType<Subtraction>(op->op))
            return op->left->unapplyVariables(l, env)
                && op->right->unapplyVariables(r, env);
    }
    return evaluated->unapplyVariables(e, env);
}
