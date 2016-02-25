#include "Multiplication.h"
#include <string>
#include "Operation.h"

Multiplication::Multiplication()
    : Operator(false, 7)
{

}

ExpPtr Multiplication::operate(ExpPtrArg first,
                               ExpPtrArg second,
                               Environment& env) const
{
    auto firstInteger  = d_cast<Integer>(first ->eval(env));
    auto secondInteger = d_cast<Integer>(second->eval(env));

    if (firstInteger && secondInteger)
        return make_ptr<Integer>(firstInteger->value *
                                         secondInteger->value);
    return make_ptr<Operation>(make_ptr<Multiplication>(),
                                       first,
                                       second);
}

std::string Multiplication::show() const
{
    return Multiplication::defaultName;
}

const std::string Multiplication::defaultName = "*";

