#include "Expressions/Pair.h"
#include "Expressions/Operation.h"

Pair::Pair()
    : Operator(true, 0, true)
{

}

ExpPtr Pair::operate(ExpPtrArg first,
                     ExpPtrArg second,
                     Environment& env)
{
    return make_ptr<Operation>(make_ptr<Pair>(), first, second);
}

std::string Pair::show() const
{
    return Pair::defaultName;
}

const std::string Pair::defaultName = ",";

