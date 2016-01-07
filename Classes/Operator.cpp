#include "Operator.h"
#include <string>

Operator::Operator(bool isRightAssociative, int priority)
    : isRightAssociative(isRightAssociative),
      priority(priority),
      name("op")
{
}

std::string Operator::show() const
{
    return "op";
}

bool Operator::isOperator(Environment* env) const
{
    return true;
}

bool Operator::unwind(ExpPtr& left,
                      ExpPtr& right,
                      ExpPtr& lvalue,
                      ExpPtr& rvalue,
                      Environment*& env)
{
    return false;
}
