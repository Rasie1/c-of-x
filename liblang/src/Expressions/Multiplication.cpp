#include "Expressions/Multiplication.h"
#include "Expressions/Operation.h"
#include "Expressions/Integer.h"
#include "Expressions/Subtraction.h"
#include "Expressions/Identifier.h"
#include "Expressions/TypeError.h"
#include "System/Environment.h"
#include "Expressions/Any.h"

Multiplication::Multiplication()
    : CalculationOperator(false, 7, true)
{

}

ExpPtr Multiplication::calculate(ExpPtrArg l, ExpPtrArg r) const
{
    if (checkType<Integer>(l) && checkType<Integer>(r))
    {
        auto firstInteger  = s_cast<Integer>(l);
        auto secondInteger = s_cast<Integer>(r);
        return make_ptr<Integer>(firstInteger->value * secondInteger->value);
    }
    else
    {
        auto operation = make_ptr<Operation>(make_ptr<Multiplication>(), l, r);
        return make_ptr<TypeError>(operation,
                                   make_ptr<Identifier>("arguments of type integer"),
                                   make_ptr<Identifier>("arguments: " + l->show() + ", " + r->show()));
    }
}

bool Multiplication::unapplyVariables(ExpPtrArg e, ExpPtrArg l, ExpPtrArg r, Environment &env) const
{
    auto lId = checkType<Identifier>(l);
    auto rId = checkType<Identifier>(r);

    if (lId && !rId)
    {
//        auto value = make_ptr<Operation>(make_ptr<Subtraction>(), e, r);
//        l->unapplyVariables(value, env);
    }
    else if (!lId && rId)
    {
//        auto value = make_ptr<Operation>(make_ptr<Subtraction>(), e, l);
//        r->unapplyVariables(value, env);
    }
    else if (rId && lId)
    {
        // can check if both values support addition
    }
    else if (!rId && !lId)
    {
        throw "nope"; //eval and add?
    }

    return true;
}

std::string Multiplication::show() const
{
    return Multiplication::defaultName;
}

const std::string Multiplication::defaultName = "*";

