#include "Operation.h"
#include "Operator.h"
#include "Union.h"
#include "Assignment.h"
#include "Identifier.h"
#include "Environment.h"

Operation::Operation(const std::shared_ptr<Operator>& op,
                     ExpPtrArg left,
                     ExpPtrArg right)
    : op(op),
      left(left),
      right(right)
{

}

ExpPtr Operation::eval(Environment*& env) const
{
    ExpPtr ret;

    DEBUG_PRINT_OP(op);
    auto l = d_cast<Identifier>(left) ? env->get(left) : left;
    if (l == nullptr)
        l = left;
    auto r = d_cast<Identifier>(right) ? env->get(right) : right;
    if (r == nullptr)
        r = right;
    DEBUG_PRINT_LT(l);
    DEBUG_PRINT_RT(r);

    ret = op->operate(left, right, env);

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
