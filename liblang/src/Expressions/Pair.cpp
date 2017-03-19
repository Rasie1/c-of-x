#include "Expressions/Pair.h"
#include "Expressions/Operation.h"
#include "System/Cast.h"

Pair::Pair()
    : Operator(true, 0, true)
{

}

Object Pair::operate(const Object& first,
                     const Object& second,
                     Environment& env)
{
    return make_ptr<Operation>(make_ptr<Pair>(), first, second);
}

std::string Pair::show() const
{
    return Pair::defaultName;
}

const std::string Pair::defaultName = ",";

