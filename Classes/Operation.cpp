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
                                              left->pattern(),
                                              right->pattern());
}

std::string Operation::toString() const
{
    return "(" +
            left->toString() +
            " " + op->toString() + " " +
            right->toString() +
            ")";
}

bool Operation::unwind(ExpPtr& lvalue,
                       ExpPtr& rvalue,
                       Environment*& env)
{
    return op->unwind(left,
                      right,
                      lvalue,
                      rvalue,
                      env);
}

bool Operation::hasNonOpVariable(Environment* env) const
{
    return left->hasNonOpVariable(env) || right->hasNonOpVariable(env);
}

}
