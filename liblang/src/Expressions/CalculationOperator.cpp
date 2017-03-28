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
    env.defaultOperator = cast<Operator>(env, thisObject());
    auto left  = Identifier::unwrapIfId(first, env);
    auto right = Identifier::unwrapIfId(second, env);

    left  = Identifier::unwrapIfId(left, env);

    right = Identifier::unwrapIfId(right, env);

    if (checkType<Any>(env, left) || checkType<Any>(env, right))
        return makeObject<Operation>(cast<Operator>(env, thisObject()), first, second);


    // Next comes union stuff that will be replaced later

    std::vector<Object> expressions;

    bool lUnion = false;
    bool rUnion = false;

    if (checkType<Operation>(env, left))
        lUnion = checkType<Union>(env, cast<Operation>(env, left)->op);
    if (checkType<Operation>(env, right))
        rUnion = checkType<Union>(env, cast<Operation>(env, right)->op);

    if (!lUnion && !rUnion)
    {
        expressions.push_back(calculate(left, right, env));
    }
    else if (lUnion && !rUnion)
    {
        auto operation = cast<Operation>(env, left);
        auto opl = operation->left;
        auto opr = operation->right;
        expressions.push_back(operate(opl, right, env));
        expressions.push_back(operate(opr, right, env));
    }
    else if (rUnion && !lUnion)
    {
        auto operation = cast<Operation>(env, right);
        auto opl = operation->left;
        auto opr = operation->right;
        expressions.push_back(operate(left, opl, env));
        expressions.push_back(operate(left, opr, env));
    }
    else
    {
        auto operationl = cast<Operation>(env, left);
        auto operationr = cast<Operation>(env, right);
        auto ll = operationl->left;
        auto lr = operationl->right;
        auto rl = operationr->left;
        auto rr = operationr->right;
        expressions.push_back(operate(ll, rl, env));
        expressions.push_back(operate(ll, rr, env));
        expressions.push_back(operate(lr, rl, env));
        expressions.push_back(operate(lr, rr, env));
    }

    return Union::make(std::begin(expressions), std::end(expressions), env);
}
