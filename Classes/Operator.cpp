#include "Operator.h"
#include "PatternOperator.h"
#include <string>

namespace Language
{

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

PatPtr Operator::pattern() const
{
    return make_ptr<PatternOperator>(
                name,
                s_cast<Operator>(
                    std::const_pointer_cast<Expression>(
                        shared_from_this())));
}

bool Operator::unwind(ExpPtr& left,
                      ExpPtr& right,
                      ExpPtr& lvalue,
                      ExpPtr& rvalue,
                      Environment*& env)
{
    return false;
}

//MakeOperator

ExpPtr MakeOperator::apply(ExpPtrArg e, Environment*& env) const
{
    return nullptr;
}

std::string MakeOperator::show() const
{
    return defaultName;
}

const std::string MakeOperator::defaultName = "operator";

//OperatorOperator

ExpPtr OperatorOperator::apply(ExpPtrArg e, Environment*& env) const
{
    return nullptr;
}

std::string OperatorOperator::show() const
{
    return static_cast<std::string>("operator{") + "variable" + "}";
}

}
