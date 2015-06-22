#include "Addition.h"
#include "Operation.h"
#include "Integer.h"

namespace Language
{

ExpPtr Addition::operate(const ExpPtr& first,
                         const ExpPtr& second,
                         Environment*& env) const
{
    auto firstInteger  = std::dynamic_pointer_cast<Integer>(first ->eval(env));
    auto secondInteger = std::dynamic_pointer_cast<Integer>(second->eval(env));

    if (firstInteger && secondInteger)
        return std::make_shared<Integer>(firstInteger->value +
                                         secondInteger->value);
    return std::make_shared<Operation>(std::make_shared<Addition>(),
                                       first,
                                       second);
}

std::string Addition::toString() const
{
    return Addition::defaultName;
}

const std::string Addition::defaultName = "+";

}
