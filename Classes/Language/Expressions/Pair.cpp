#include "Pair.h"
#include "Operation.h"

Pair::Pair()
    : Operator(true, 0)
{

}

ExpPtr Pair::operate(ExpPtrArg first,
                     ExpPtrArg second,
                     Environment& env) const
{
    return make_ptr<Operation>(make_ptr<Pair>(), first, second);
}

std::string Pair::show() const
{
    return Pair::defaultName;
}

const std::string Pair::defaultName = ",";

