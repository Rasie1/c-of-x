#include "Operation.h"
#include "Operator.h"
#include "Union.h"
#include "Assignment.h"
#include "Variable.h"
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

    // warning, env is not copied
    if (!d_cast<Assignment>(op))
    {
        auto lVariable = d_cast<Identifier>(left);
        auto rVariable = d_cast<Identifier>(right);

        auto left  = this->left;
        auto right = this->right;

        if (lVariable)
            left  = env->get(this->left);
        if (rVariable)
            right = env->get(this->right);

        if (left  == nullptr)
            left  = this->left;
        if (right == nullptr)
            right = this->right;

        DEBUG_PRINT_OP(op);
        DEBUG_PRINT_LT(left);
        DEBUG_PRINT_RT(right);

        auto operationLeft  = d_cast<Operation>(left);
        auto operationRight = d_cast<Operation>(right);
        if (operationLeft && d_cast<Union>(operationLeft->op))
        {
            if (operationRight && d_cast<Union>(operationRight->op))
                ret = make_ptr<Operation>(make_ptr<Union>(),
                                          make_ptr<Operation>(make_ptr<Union>(),
                                                              op->operate(operationLeft->left, operationRight->left, env),
                                                              op->operate(operationLeft->left, operationRight->right, env)),
                                          make_ptr<Operation>(make_ptr<Union>(),
                                                              op->operate(operationLeft->right, operationRight->left, env),
                                                              op->operate(operationLeft->right, operationRight->right, env)));
            else
                ret = make_ptr<Operation>(make_ptr<Union>(),
                                          op->operate(operationLeft->left, right, env),
                                          op->operate(operationLeft->right, right, env));

        }
        else if (operationRight && d_cast<Union>(operationRight->op))
            ret = make_ptr<Operation>(make_ptr<Union>(),
                                      op->operate(left, operationRight->left, env),
                                      op->operate(left, operationRight->right, env));
        else
            ret = op->operate(left, right, env);
    }
    else
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
