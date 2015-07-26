#include "Subtraction.h"
#include <string>
#include "Operation.h"
#include "Addition.h"

namespace Language
{

Subtraction::Subtraction()
    : Operator(false, 5)
{
}

ExpPtr Subtraction::operate(ExpPtrArg first,
                            ExpPtrArg second,
                            Environment*& env) const
{
    auto firstInteger  = d_cast<Integer>(first ->eval(env));
    auto secondInteger = d_cast<Integer>(second->eval(env));

    if (firstInteger && secondInteger)
        return make_ptr<Integer>(firstInteger->value -
                                         secondInteger->value);
    return make_ptr<Operation>(make_ptr<Subtraction>(),
                                       first,
                                       second);
}

std::string Subtraction::toString() const
{
    return Subtraction::defaultName;
}

const std::string Subtraction::defaultName = "-";


bool Subtraction::unwind(ExpPtr& left,
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
                    make_ptr<Addition>(),
                    rvalue,
                    right);
        return true;
    }
    else if (right->hasNonOpVariable(env))
    {
        lvalue = right;
        rvalue = make_ptr<Operation>(
                    make_ptr<Subtraction>(),
                    left,
                    rvalue);
        return true;
    }
    return false;
}


}
