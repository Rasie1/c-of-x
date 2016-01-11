#include "Addition.h"
#include "Operation.h"
#include "Integer.h"
#include "Subtraction.h"
#include "Variable.h"
#include "TypeError.h"

Addition::Addition()
    : Operator(false, 5)
{
}

ExpPtr Addition::operate(ExpPtrArg first,
                         ExpPtrArg second,
                         Environment*& env) const
{
    auto l = first->eval(env);
    auto r = second->eval(env);
    auto firstInteger  = d_cast<Integer>(l);
    auto secondInteger = d_cast<Integer>(r);

    if (!firstInteger || !secondInteger)
    {
        auto operation = make_ptr<Operation>(make_ptr<Addition>(), l, r);
        return make_ptr<TypeError>(operation,
                                   make_ptr<Identifier>("arguments of type integer"),
                                   make_ptr<Identifier>("arguments: " + l->show() + ", " + r->show()));
    }

    return make_ptr<Integer>(firstInteger->value + secondInteger->value);
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
                      Environment*& env)
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
