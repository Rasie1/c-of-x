#include "Operator.h"
#include "PatternOperator.h"

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

bool Operator::isOperator(Environment* env) const
{
    return true;
}

PatPtr Operator::pattern() const
{
    return std::make_shared<PatternOperator>(
                name,
                std::static_pointer_cast<Operator>(
                    std::const_pointer_cast<Expression>(
                        shared_from_this())));
}

PatPtr Operator::leftPattern(const ExpPtr& e) const
{
    return e->pattern();
}

PatPtr Operator::rightPattern(const ExpPtr& e) const
{
    return e->pattern();
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
