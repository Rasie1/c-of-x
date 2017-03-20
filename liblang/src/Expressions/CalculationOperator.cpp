#include "Expressions/CalculationOperator.h"
#include "Expressions/Operation.h"
#include "Expressions/Integer.h"
#include "Expressions/Subtraction.h"
#include "Expressions/Identifier.h"
#include "Expressions/TypeError.h"
#include "System/Environment.h"
#include "Expressions/Union.h"
#include "Expressions/Any.h"
#include "System/Cast.h"
#include <vector>

CalculationOperator::CalculationOperator(bool isRightAssociative, int priority, bool splitting)
    : Operator(isRightAssociative, priority, splitting)
{
}

Object CalculationOperator::operate(const Object& first,
                                    const Object& second,
                                    Environment& env)
{
    env.defaultOperator = cast<Operator>(thisObject());
    auto left  = Identifier::unwrapIfId(first, env);
    auto right = Identifier::unwrapIfId(second, env);

    auto envCopy = env;
    left  = left->eval(envCopy);
    left  = Identifier::unwrapIfId(left, envCopy);

    envCopy = env;
    right = right->eval(envCopy);
    right = Identifier::unwrapIfId(right, envCopy);

    if (checkType<Any>(left) || checkType<Any>(right))
        return makeObject<Operation>(cast<Operator>(thisObject()), first, second);


    // Next comes union stuff that will be replaced later

    std::vector<Object> expressions;

    bool lUnion = false;
    bool rUnion = false;

    if (checkType<Operation>(left))
        lUnion = checkType<Union>(cast<Operation>(left)->op);
    if (checkType<Operation>(right))
        rUnion = checkType<Union>(cast<Operation>(right)->op);

    if (!lUnion && !rUnion)
    {
        expressions.push_back(calculate(left, right));
    }
    else if (lUnion && !rUnion)
    {
        auto operation = cast<Operation>(left);
        auto opl = operation->left;
        auto opr = operation->right;
        expressions.push_back(operate(opl, right, env));
        expressions.push_back(operate(opr, right, env));
    }
    else if (rUnion && !lUnion)
    {
        auto operation = cast<Operation>(right);
        auto opl = operation->left;
        auto opr = operation->right;
        expressions.push_back(operate(left, opl, env));
        expressions.push_back(operate(left, opr, env));
    }
    else
    {
        auto operationl = cast<Operation>(left);
        auto operationr = cast<Operation>(right);
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
