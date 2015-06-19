#include "Operator.h"

namespace Language
{

Operator::Operator(bool isRightAssociative)
    : isRightAssociative(isRightAssociative),
      name("op")
{
}

std::string Operator::toString() const
{
    return "op";
}
/*
PrecedenceInfo::PrecedenceInfo(Operator *more, Operator *less)
    : more(more),
      less(less)
{

}

std::string PrecedenceInfo::toString() const
{
    return more->toString() + " > " + less->toString();
}
*/
}
