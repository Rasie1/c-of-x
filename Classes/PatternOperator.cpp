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

bool PatternOperator::match(ExpPtrArg other) const
{
    auto y = d_cast<PatternVariable>(other);
    if (y)
        return y->name == name;
    auto x = d_cast<PatternOperator>(other);
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


PatternOperation::PatternOperation(PatPtrArg op,
                                   PatPtrArg left,
                                   PatPtrArg right)
    : op(op),
      left(left),
      right(right)
{

}

bool PatternOperation::match(ExpPtrArg other) const
{
    auto x = d_cast<PatternOperation>(other);
    if (!x)
        return false;
    if (!op->match(x->op))
        return false;
    if (!left->match(x->left))
        return false;
    if (!right->match(x->right))
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
