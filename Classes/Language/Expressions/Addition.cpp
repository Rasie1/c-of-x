#include "Addition.h"
#include "Operation.h"
#include "Integer.h"
#include "Subtraction.h"
#include "Identifier.h"
#include "TypeError.h"
#include "Environment.h"
#include "Any.h"

Addition::Addition()
    : CalculationOperator(false, 5, true)
{
}

ExpPtr Addition::calculate(ExpPtrArg l, ExpPtrArg r) const
{
    if (checkType<Integer>(l) && checkType<Integer>(r))
    {
        auto firstInteger  = s_cast<Integer>(l);
        auto secondInteger = s_cast<Integer>(r);
        return make_ptr<Integer>(firstInteger->value + secondInteger->value);
    }
    else
    {
        auto operation = make_ptr<Operation>(make_ptr<Addition>(), l, r);
        return make_ptr<TypeError>(operation,
                                   make_ptr<Identifier>("arguments of type integer"),
                                   make_ptr<Identifier>("arguments: " + l->show() + ", " + r->show()));
    }
}

std::string Addition::show() const
{
    return Addition::defaultName;
}

const std::string Addition::defaultName = "+";

bool Addition::unapplyVariables(ExpPtrArg e, ExpPtrArg l, ExpPtrArg r, Environment &env) const
{
    auto lId = checkType<Identifier>(l);
    auto rId = checkType<Identifier>(r);

    if (lId && !rId)
    {
        auto value = make_ptr<Operation>(make_ptr<Subtraction>(), e, r);
        l->unapplyVariables(value, env);
    }
    else if (!lId && rId)
    {
        auto value = make_ptr<Operation>(make_ptr<Subtraction>(), e, l);
        r->unapplyVariables(value, env);
    }
    else if (rId && lId)
    {
        // can check if both values support addition
//        throw std::runtime_error("Not Implemented: Addition unapply third case");
    }
    else if (!rId && !lId)
    {
        if (checkType<Operation>(e))
        {
            auto op = s_cast<Operation>(e);
            if (checkType<Addition>(op->op))
                return op->left->unapplyVariables(l, env)
                    && op->right->unapplyVariables(r, env);
        }
        return false;
    }

    return true;
}
