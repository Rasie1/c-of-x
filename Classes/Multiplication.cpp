#include "Multiplication.h"
#include <string>
#include "Operation.h"

namespace Language
{

Multiplication::Multiplication()
    : Operator(false, 7)
{

}

ExpPtr Multiplication::operate(const ExpPtr& first,
                               const ExpPtr& second,
                               Environment*& env) const
{
    auto firstInteger  = std::dynamic_pointer_cast<Integer>(first ->eval(env));
    auto secondInteger = std::dynamic_pointer_cast<Integer>(second->eval(env));

    if (firstInteger && secondInteger)
        return std::make_shared<Integer>(firstInteger->value *
                                         secondInteger->value);
    return std::make_shared<Operation>(std::make_shared<Multiplication>(),
                                       first,
                                       second);
}

std::string Multiplication::toString() const
{
    return Multiplication::defaultName;
}

const std::string Multiplication::defaultName = "*";

}
