#include "Subtraction.h"
#include <string>
#include "Operation.h"

namespace Language
{

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

}
