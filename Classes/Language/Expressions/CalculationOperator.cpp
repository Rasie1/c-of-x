#include "CalculationOperator.h"
#include "Operation.h"
#include "Integer.h"
#include "Subtraction.h"
#include "Identifier.h"
#include "TypeError.h"
#include "Environment.h"
#include "Union.h"
#include "Any.h"
#include <vector>

CalculationOperator::CalculationOperator(bool isRightAssociative, int priority, bool splitting)
    : Operator(isRightAssociative, priority, splitting)
{
}

ExpPtr CalculationOperator::operate(ExpPtrArg first,
                                    ExpPtrArg second,
                                    Environment& envOld) const
{
    Environment env = envOld;
    env.defaultOperator = s_cast<const Operator>(shared_from_this());
    auto left  = Identifier::unwrapIfId(first, env);
    auto right = Identifier::unwrapIfId(second, env);
    left  = left->eval(env);
    right = right->eval(env);
    left  = Identifier::unwrapIfId(left, env);
    right = Identifier::unwrapIfId(right, env);

    if (checkType<Any>(left) || checkType<Any>(right))
        return make_ptr<Operation>(s_cast<const Operator>(shared_from_this()), left, right);


    // Next comes union stuff that will be replaced later

    std::vector<ExpPtr> expressions;

    bool lUnion = false;
    bool rUnion = false;

    if (checkType<Operation>(left))
        lUnion = checkType<Union>(s_cast<Operation>(left)->op);
    if (checkType<Operation>(right))
        rUnion = checkType<Union>(s_cast<Operation>(right)->op);

    if (!lUnion && !rUnion)
    {
        expressions.push_back(calculate(left, right));
    }
    else if (lUnion && !rUnion)
    {
        auto operation = s_cast<Operation>(left);
        auto opl = operation->left;
        auto opr = operation->right;
        expressions.push_back(operate(opl, right, env));
        expressions.push_back(operate(opr, right, env));
    }
    else if (rUnion && !lUnion)
    {
        auto operation = s_cast<Operation>(right);
        auto opl = operation->left;
        auto opr = operation->right;
        expressions.push_back(operate(left, opl, env));
        expressions.push_back(operate(left, opr, env));
    }
    else
    {
        auto operationl = s_cast<Operation>(left);
        auto operationr = s_cast<Operation>(right);
        auto ll = operationl->left;
        auto lr = operationl->right;
        auto rl = operationr->left;
        auto rr = operationr->right;
        expressions.push_back(operate(ll, rl, env));
        expressions.push_back(operate(ll, rr, env));
        expressions.push_back(operate(lr, rl, env));
        expressions.push_back(operate(lr, rr, env));
    }

    return Union::make(std::begin(expressions), std::end(expressions));
}
