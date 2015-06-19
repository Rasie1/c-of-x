#include "Multiplication.h"
#include <string>
#include "Operation.h"

namespace Language
{

Expression* Multiplication::operate(Expression* first,
                                    Expression* second,
                                    Environment*& env)
{
    auto firstInteger  = dynamic_cast<Integer*>(first ->eval(env));
    auto secondInteger = dynamic_cast<Integer*>(second->eval(env));

    if (firstInteger && secondInteger)
        return new Integer(firstInteger->value *
                           secondInteger->value);
    return new Operation(new Multiplication(), first, second);
}

std::string Multiplication::toString() const
{
    return Multiplication::defaultName;
}

const std::string Multiplication::defaultName = "*";

}
