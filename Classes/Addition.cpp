#include "Addition.h"
#include "Operation.h"
#include "Integer.h"
#include "Environment.h"
#include "Subtraction.h"
#include "Variable.h"

namespace Language
{

Addition::Addition()
    : Operator(false, 5)
{
}


ExpPtr Addition::operate(ExpPtrArg first,
                         ExpPtrArg second,
                         Environment*& env) const
{
    auto firstInteger  = std::dynamic_pointer_cast<Integer>(first ->eval(env));
    auto secondInteger = std::dynamic_pointer_cast<Integer>(second->eval(env));

    if (firstInteger && secondInteger)
        return make_ptr<Integer>(firstInteger->value +
                                         secondInteger->value);
    return make_ptr<Operation>(make_ptr<Addition>(),
                                       first,
                                       second);
}

std::string Addition::toString() const
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


}
