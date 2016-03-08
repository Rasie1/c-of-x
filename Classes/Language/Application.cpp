#include "Application.h"
#include <vector>
#include "Environment.h"
#include "Lambda.h"
#include "Operation.h"
#include "Error.h"
#include "Function.h"
#include "Identifier.h"
#include "Union.h"
#include "Void.h"
#include "Any.h"
#include "Quote.h"
#include "Closure.h"

Application::Application()
    : Operator(false, 10)
{
}

bool isExpressionQuoted(ExpPtrArg left, Environment& env)
{
    if (checkType<Quote>(left))
        return true;
    if (checkType<Closure>(left))
    {
        auto cl = s_cast<Closure>(left);
        if (checkType<Operation>(cl->argument))
        {
            auto op = s_cast<Operation>(cl->argument);
            if (checkType<Application>(op->op))
            {
                if (checkType<Quote>(Identifier::unwrapIfId(op->left, env)))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

ExpPtr Application::operate(ExpPtrArg first,
                            ExpPtrArg second,
                            Environment& env) const
{
    ExpPtr left, right;

    left = Identifier::unwrapIfId(first, env);

    left = left->eval(env);
    if (isExpressionQuoted(left, env))
        right = second;
    else
        right = second->eval(env);

    auto calculate = [&env](ExpPtrArg l, ExpPtrArg r) -> ExpPtr
    {
        auto function = d_cast<Function>(l);

        if (!function)
            return make_ptr<ErrorWithMessage>("Not a function");

        return function->apply(r, env);
    };

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

std::string Application::show() const
{
    return "";
}

bool Application::unapplyVariables(ExpPtrArg e, ExpPtrArg l, ExpPtrArg r, Environment &env) const
{
    if (checkType<Identifier>(l))
    {
        auto lvalue = env.getEqual(l);
        if (checkType<Quote>(lvalue))
        {
            auto q = s_cast<Quote>(lvalue);
            auto qe = s_cast<QuotedExpression>(q->apply(r, env));
            return qe->unapplyVariables(e, env);
        }
        if (checkType<Any>(lvalue))
        {
            // for recursion to work
            auto newEnv = env;
            newEnv.addEqual(l, make_ptr<Operation>(make_ptr<Lambda>(), r, e));
            auto closure = Lambda().operate(r, e, newEnv);

            env.addEqual(l, closure);

            return true;
        }
        else if (std::shared_ptr<ReversibleFunction> f = d_cast<ReversibleFunction>(lvalue))
        {
            return f->unapplyVariables(e, r, env);
        }
        return false;
    }
    else
    {
        auto closure = make_ptr<Operation>(make_ptr<Lambda>(), r, e);
        return l->unapplyVariables(closure, env);
    }

    return true;
}
