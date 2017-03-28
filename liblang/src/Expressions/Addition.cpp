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

Object Addition::calculate(const Object& l, 
                           const Object& r,
                           Environment& env) const
{
    if (auto first = cast<Integer>(env, l))
    if (auto second = cast<Integer>(env, r))
        return makeObject<Integer>(first->value + second->value);
        
    auto operation = makeOperation<Addition>(l, r);
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
    auto lId = checkType<Identifier>(env, l);
    auto rId = checkType<Identifier>(env, r);

    if (lId && !rId)
    {
        auto value = makeOperation<Subtraction>(e, r);
        return l->unapplyVariables(value, env);
    }
    else if (!lId && rId)
    {
        auto value = makeOperation<Subtraction>(e, l);
        return r->unapplyVariables(value, env);
    }

    auto evaluated = makeOperation<Addition>(l, r);
    if (auto op = cast<Operation>(env, evaluated))
    {
        if (checkType<Addition>(env, op->op))
            return op->left->unapplyVariables(l, env)
                && op->right->unapplyVariables(r, env);
    }
    return evaluated->unapplyVariables(e, env);
}
