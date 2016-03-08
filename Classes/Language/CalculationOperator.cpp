#include "CalculationOperator.h"
#include "Operation.h"
#include "Integer.h"
#include "Subtraction.h"
#include "Identifier.h"
#include "TypeError.h"
#include "Environment.h"
#include "Union.h"
#include "Any.h"

CalculationOperator::CalculationOperator(bool isRightAssociative, int priority)
    : Operator(isRightAssociative, priority)
{
}

ExpPtr CalculationOperator::operate(ExpPtrArg first,
                                    ExpPtrArg second,
                                    Environment& env) const
{
    ExpPtr l, r;
    l = Identifier::unwrapIfId(first, env);
    r = Identifier::unwrapIfId(second, env);
    auto left  = l->eval(env);
    auto right = r->eval(env);

    if (checkType<Any>(left) || checkType<Any>(right))
        return make_ptr<Operation>(s_cast<const Operator>(shared_from_this()), left, right);

    // Handle indetermenism
    auto operationLeft  = d_cast<Operation>(left);
    auto operationRight = d_cast<Operation>(right);

    std::vector<ExpPtr> expressions;
    expressions.reserve(4);

    if (operationLeft && checkType<Union>(operationLeft->op))
        if (operationRight && checkType<Union>(operationRight->op))
        {
            expressions.push_back(calculate(operationLeft->left,
                                            operationRight->left));
            expressions.push_back(calculate(operationLeft->left,
                                            operationRight->right));
            expressions.push_back(calculate(operationLeft->right,
                                            operationRight->left));
            expressions.push_back(calculate(operationLeft->right,
                                            operationRight->right));
        }
        else
        {
            expressions.push_back(calculate(operationLeft->left, right));
            expressions.push_back(calculate(operationLeft->right, right));
        }
    else if (operationRight && checkType<Union>(operationRight->op))
    {
        expressions.push_back(calculate(left, operationRight->left));
        expressions.push_back(calculate(left, operationRight->right));
    }
    else
        return calculate(left, right);

    return Union::make(expressions);
}
