#include "Subtraction.h"
#include <string>
#include "Operation.h"

namespace Language
{

Expression* Subtraction::operate(Expression* first,
                              Expression* second,
                              Environment*& env)
{
    auto firstInteger  = dynamic_cast<Integer*>(first ->eval(env));
    auto secondInteger = dynamic_cast<Integer*>(second->eval(env));

    if (firstInteger && secondInteger)
        return new Integer(firstInteger->value -
                           secondInteger->value);
    return new Operation(new Subtraction(), first, second);
}

std::string Subtraction::toString() const
{
    return Subtraction::defaultName;
}

const std::string Subtraction::defaultName = "-";

}
