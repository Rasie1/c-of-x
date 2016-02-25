#include "Addition.h"
#include "Operation.h"
#include "Integer.h"
#include "Subtraction.h"
#include "Identifier.h"
#include "TypeError.h"
#include "Environment.h"

Addition::Addition()
    : CalculationOperator(false, 5)
{
}

ExpPtr Addition::calculate(ExpPtrArg l, ExpPtrArg r) const
{
    if (typeid(*l) == typeid(Integer) && typeid(*r) == typeid(Integer))
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

bool Addition::unwind(ExpPtr& left,
                      ExpPtr& right,
                      ExpPtr& lvalue,
                      ExpPtr& rvalue,
                      Environment& env)
{
    auto newEnv = env;
    if (left->hasNonOpVariable(env))
    {
        if (right->hasNonOpVariable(env))
            return false;
        lvalue = left;
        rvalue = make_ptr<Operation>(
                    make_ptr<Subtraction>(),
                    rvalue,
                    right);
        return true;
    }
    else if (right->hasNonOpVariable(env))
    {
        lvalue = right;
        rvalue = make_ptr<Operation>(
                    make_ptr<Subtraction>(),
                    rvalue,
                    left);
        return true;
    }
    return false;
}
