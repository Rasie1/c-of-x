#include "PatternOperator.h"
#include "PatternVariable.h"
#include "Operator.h"

namespace Language
{

PatternOperator::PatternOperator(const std::string& name,
                                 const std::shared_ptr<Operator>& op)
    : name(name),
      op(op)
{

}

bool PatternOperator::match(const ExpPtr& other,
                            Environment* env) const
{
    auto y = std::dynamic_pointer_cast<PatternVariable>(other);
    if (y)
        return y->name == name;
    auto x = std::dynamic_pointer_cast<PatternOperator>(other);
    if (x)
        return x->name == name;
    return false;
}

bool PatternOperator::isOperator(Environment* env) const
{
    return true;
}

std::string PatternOperator::toString() const
{
    return op->toString();
}

bool PatternOperator::isMoreThan(const std::shared_ptr<PatternOperator>& other,
                                 Environment* env) const
{
    // ???
    return false;
}


PatternOperation::PatternOperation(const PatPtr& op,
                                   const PatPtr& left,
                                   const PatPtr& right)
    : op(op),
      left(left),
      right(right)
{

}

bool PatternOperation::match(const ExpPtr& other,
                             Environment* env) const
{
    auto x = std::dynamic_pointer_cast<PatternOperation>(other);
    if (!x)
        return false;
    if (!op->match(x->op, env))
        return false;
    if (!left->match(x->left, env))
        return false;
    if (!right->match(x->right, env))
        return false;
    return true;
}

std::string PatternOperation::toString() const
{
    return "(" + left->toString() + " " +
            op->toString()
            + " " + right->toString() + ")";
}

}
