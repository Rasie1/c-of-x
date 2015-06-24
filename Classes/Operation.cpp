#include "Operation.h"
#include "Operator.h"
#include "PatternOperator.h"

namespace Language
{

Operation::Operation(const std::shared_ptr<Operator>& op,
                     const ExpPtr& left,
                     const ExpPtr& right)
    : op(op),
      left(left),
      right(right)
{

}

Operation::~Operation()
{
}

ExpPtr Operation::evaluation(Environment*& env) const
{
    DEBUG_PRINT_OP(op);
    DEBUG_PRINT_LT(left);
    DEBUG_PRINT_RT(right);
    auto ret = op->operate(left, right, env);
    DEBUG_PRINT_RS(ret);
    return ret;
}

PatPtr Operation::pattern() const
{
    return std::make_shared<PatternOperation>(op->pattern(),
                                              op->leftPattern(left),
                                              op->rightPattern(right));
}

std::string Operation::toString() const
{
    return "(" +
            left->toString() +
            " " + op->toString() + " " +
            right->toString() +
            ")";
}


}
