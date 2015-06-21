#include "Addition.h"
#include <string>
#include "Operation.h"

namespace Language
{

Expression* Addition::operate(Expression* first,
                              Expression* second,
                              Environment*& env)
{
    auto firstInteger  = dynamic_cast<Integer*>(first ->eval(env));
    auto secondInteger = dynamic_cast<Integer*>(second->eval(env));

    if (firstInteger && secondInteger)
        return new Integer(firstInteger->value +
                           secondInteger->value);
    return new Operation(new Addition(),
                         first,
                         second);
}

std::string Addition::toString() const
{
    return Addition::defaultName;
}

const std::string Addition::defaultName = "+";

}
