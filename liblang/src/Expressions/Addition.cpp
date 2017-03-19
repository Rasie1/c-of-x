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

Object Addition::calculate(const Object& l, const Object& r) const
{
    if (auto first = d_cast<Integer>(l))
    if (auto second = d_cast<Integer>(r))
        return makeObject<Integer>(first->value + second->value);
        
    auto operation = makeObject<Operation>(makeObject<Addition>(), l, r);
    return makeObject<TypeError>(operation,
                               makeObject<Identifier>("arguments of type integer"),
                               makeObject<Identifier>("arguments: " + l->show() + ", " + r->show()));
}

std::string Addition::show() const
{
    return Addition::defaultName;
}

const std::string Addition::defaultName = "+";

bool Addition::unapplyVariables(const Object& e, const Object& l, const Object& r, Environment &env)
{
    auto lId = checkType<Identifier>(l);
    auto rId = checkType<Identifier>(r);

    if (lId && !rId)
    {
        auto value = makeObject<Operation>(makeObject<Subtraction>(), e, r);
        return l->unapplyVariables(value, env);
    }
    else if (!lId && rId)
    {
        auto value = makeObject<Operation>(makeObject<Subtraction>(), e, l);
        return r->unapplyVariables(value, env);
    }

    auto evaluated = makeObject<Operation>(makeObject<Addition>(),
                                         l, r)->eval(env);
    if (auto op = d_cast<Operation>(evaluated))
    {
        if (checkType<Addition>(op->op))
            return op->left->unapplyVariables(l, env)
                && op->right->unapplyVariables(r, env);
    }
    return evaluated->unapplyVariables(e, env);
}
