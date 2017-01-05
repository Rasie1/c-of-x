#include "Subtraction.h"
#include <string>
#include "Integer.h"
#include "Operation.h"
#include "Addition.h"
#include "Identifier.h"
#include "TypeError.h"
#include "Environment.h"

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
        l->unapplyVariables(value, env);
    }
    else if (!lId && rId)
    {
        auto value = make_ptr<Operation>(make_ptr<Addition>(), e, l);
        r->unapplyVariables(value, env);
    }
    else if (rId && lId)
    {
        // can check if both values support addition
    }
    else if (!rId && !lId)
    {
        // possible there is still operations
        throw "nope"; //eval and add?
    }

    return true;
}
