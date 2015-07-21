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

ExpPtr Subtraction::operate(const ExpPtr& first,
                            const ExpPtr& second,
                            Environment*& env) const
{
    auto firstInteger  = std::dynamic_pointer_cast<Integer>(first ->eval(env));
    auto secondInteger = std::dynamic_pointer_cast<Integer>(second->eval(env));

    if (firstInteger && secondInteger)
        return std::make_shared<Integer>(firstInteger->value -
                                         secondInteger->value);
    return std::make_shared<Operation>(std::make_shared<Subtraction>(),
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
        rvalue = std::make_shared<Operation>(
                    std::make_shared<Addition>(),
                    rvalue,
                    right);
        return true;
    }
    else if (right->hasNonOpVariable(env))
    {
        lvalue = right;
        rvalue = std::make_shared<Operation>(
                    std::make_shared<Subtraction>(),
                    left,
                    rvalue);
        return true;
    }
    return false;
}


}
