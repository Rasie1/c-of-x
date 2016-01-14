#include "CalculationOperator.h"
#include "Operation.h"
#include "Integer.h"
#include "Subtraction.h"
#include "Identifier.h"
#include "TypeError.h"
#include "Environment.h"
#include "Union.h"

CalculationOperator::CalculationOperator(bool isRightAssociative, int priority)
    : Operator(isRightAssociative, priority)
{
}

ExpPtr CalculationOperator::operate(ExpPtrArg first,
                                    ExpPtrArg second,
                                    Environment*& env) const
{
    ExpPtr l, r, ret;
    l = d_cast<Identifier>(first);
    if (l)
        l = env->get(l);
    else
        l = first;
    r = d_cast<Identifier>(second);
    if (r)
        r = env->get(r);
    else
        r = second;
    auto left  = l->eval(env);
    auto right = r->eval(env);

    // Handle indetermenism
    auto operationLeft  = d_cast<Operation>(left);
    auto operationRight = d_cast<Operation>(right);
    if (operationLeft && d_cast<Union>(operationLeft->op))
        if (operationRight && d_cast<Union>(operationRight->op))
            ret = make_ptr<Operation>(
                        make_ptr<Union>(),
                        make_ptr<Operation>(make_ptr<Union>(),
                                            calculate(operationLeft->left,
                                                      operationRight->left),
                                            calculate(operationLeft->left,
                                                      operationRight->right)),
                        make_ptr<Operation>(make_ptr<Union>(),
                                            calculate(operationLeft->right,
                                                      operationRight->left),
                                            calculate(operationLeft->right,
                                                      operationRight->right)));
        else
            ret = make_ptr<Operation>(make_ptr<Union>(),
                                      calculate(operationLeft->left, right),
                                      calculate(operationLeft->right, right));

    else if (operationRight && d_cast<Union>(operationRight->op))
        ret = make_ptr<Operation>(make_ptr<Union>(),
                                  calculate(left, operationRight->left),
                                  calculate(left, operationRight->right));
    else
        ret = calculate(left, right);

    return ret;
}
