#include "Expressions/Addition.h"
#include "Expressions/Operation.h"
#include "Expressions/Integer.h"
#include "Expressions/Subtraction.h"
#include "Expressions/Identifier.h"
#include "Expressions/TypeError.h"
#include "System/Environment.h"
#include "Expressions/Any.h"
#include "System/Cast.h"

Addition::Addition()
    : CalculationOperator(false, 5, true)
{
}

ExpPtr Addition::calculate(ExpPtrArg l, ExpPtrArg r) const
{
    if (auto first = d_cast<Integer>(l))
    if (auto second = d_cast<Integer>(r))
        return make_ptr<Integer>(first->value + second->value);
        
    auto operation = make_ptr<Operation>(make_ptr<Addition>(), l, r);
    return make_ptr<TypeError>(operation,
                               make_ptr<Identifier>("arguments of type integer"),
                               make_ptr<Identifier>("arguments: " + l->show() + ", " + r->show()));
}

std::string Addition::show() const
{
    return Addition::defaultName;
}

const std::string Addition::defaultName = "+";

bool Addition::unapplyVariables(ExpPtrArg e, ExpPtrArg l, ExpPtrArg r, Environment &env)
{
    auto lId = checkType<Identifier>(l);
    auto rId = checkType<Identifier>(r);

    if (lId && !rId)
    {
        auto value = make_ptr<Operation>(make_ptr<Subtraction>(), e, r);
        return l->unapplyVariables(value, env);
    }
    else if (!lId && rId)
    {
        auto value = make_ptr<Operation>(make_ptr<Subtraction>(), e, l);
        return r->unapplyVariables(value, env);
    }

    auto evaluated = make_ptr<Operation>(make_ptr<Addition>(),
                                         l, r)->eval(env);
    if (auto op = d_cast<Operation>(evaluated))
    {
        if (checkType<Addition>(op->op))
            return op->left->unapplyVariables(l, env)
                && op->right->unapplyVariables(r, env);
    }
    return evaluated->unapplyVariables(e, env);
}
