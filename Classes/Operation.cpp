#include "Operation.h"
#include "Operator.h"


Operation::Operation(const std::shared_ptr<Operator>& op,
                     ExpPtrArg left,
                     ExpPtrArg right)
    : op(op),
      left(left),
      right(right)
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

std::string Operation::show() const
{
    return "(" +
            left->show() +
            " " + op->show() + " " +
            right->show() +
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
