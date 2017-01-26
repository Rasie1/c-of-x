#include "Expressions/Application.h"
#include <vector>
#include "System/Environment.h"
#include "Expressions/Lambda.h"
#include "Expressions/Operation.h"
#include "Expressions/Error.h"
#include "Expressions/Morphism.h"
#include "Expressions/Identifier.h"
#include "Expressions/Union.h"
#include "Expressions/Void.h"
#include "Expressions/Any.h"
#include "Expressions/Quote.h"
#include "Expressions/Closure.h"
#include "Expressions/Equality.h"

Application::Application()
    : Operator(false, 10, true)
{
}

static bool isExpressionQuoted(ExpPtrArg left, Environment& env)
{
    if (checkType<Quote>(left))
        return true;
    if (auto cl = d_cast<Closure>(left))
    {
        if (checkType<QuotedExpression>(cl->argument))
            return true;
        if (auto op = d_cast<Operation>(cl->argument))
        {
            if (checkType<Application>(op->op))
            {
                if (checkType<Quote>(Identifier::unwrapIfId(op->left, env)))
                {
                    return true;
                }
            }
        }
    }
    if (checkType<QuotedExpression>(left))
        return true;

    return false;
}

ExpPtr Application::operate(ExpPtrArg first,
                            ExpPtrArg second,
                            Environment& env) const
{
    ExpPtr left, right;
    left = Identifier::unwrapIfId(first, env)->eval(env);
    if (isExpressionQuoted(left, env))
        right = second;
    else
        right = second->eval(env);

    if (checkType<Any>(left) || checkType<Any>(right))
        return make_ptr<Operation>(s_cast<const Operator>(shared_from_this()), left, right);


    // Next comes union stuff that will be replaced later

    std::vector<ExpPtr> expressions;

    bool lUnion = false;
    bool rUnion = false;

    if (auto l = d_cast<Operation>(left))
        lUnion = checkType<Union>(l->op);
    if (auto r = d_cast<Operation>(right))
        rUnion = checkType<Union>(r->op);

    if (!lUnion && !rUnion)
    {
//        if (checkType<Equals>(left) && checkType<Equals>(right))
//        {
//            auto eq = s_cast<Equals>(left);
//            auto eq = s_cast<Equals>(left);
//            left = eq->value;

//        }
//        else
        {
            
            if (auto function = d_cast<Morphism>(left))
            {
                expressions.push_back(function->apply(right, env));
            }
            //                                                TODO: save env?
            else if (auto function = d_cast<Morphism>(left->eval(env)))
            {
                expressions.push_back(function->apply(right, env));
            }
            else
                return make_ptr<ErrorWithMessage>("Not a function");
        }
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

std::string Application::show() const
{
    return "";
}

bool Application::unapplyVariables(ExpPtrArg e,
                                   ExpPtrArg l,
                                   ExpPtrArg r,
                                   Environment &env) const
{
    auto lId = checkType<Identifier>(l);
    auto rId = checkType<Identifier>(r);

    auto lvalue = lId ? env.getEqual(l) : l;

    if (auto q = d_cast<Quote>(lvalue))
    {
        auto qe = s_cast<QuotedExpression>(q->apply(r, env));
        return qe->unapplyVariables(e, env);
    }
    if (auto f = d_cast<Morphism>(lvalue))
    {
        auto inverse = d_cast<Morphism>(f->inverse());
        if (!checkType<Void>(inverse))
        {
            auto inversed = inverse->apply(r, env);
            return inversed->unapplyVariables(e, env);
        }
    }
    if (lId)
    {
        auto newEnv = env;
        newEnv.addEqual(l, make_ptr<Operation>(make_ptr<Lambda>(), r, e), true);
        auto closure = Lambda().operate(r, e, newEnv);

        auto ret = env.addEqual(l, closure, false);

        return !checkType<Void>(ret);
    }

    // Otherwise, 'unapply' the application, turning right side into function
    // For example, `f x = y ----> f = x -> y`
    auto closure = make_ptr<Operation>(make_ptr<Lambda>(), r, e);
    return l->unapplyVariables(closure, env);
}


ReverseApplication::ReverseApplication()
    : Operator(false, 10)
{
}

ExpPtr ReverseApplication::operate(ExpPtrArg first,
                                   ExpPtrArg second,
                                   Environment& env) const
{
    return proxy.operate(second, first, env);
}

std::string ReverseApplication::show() const
{
    return ReverseApplication::defaultName;
}

const std::string ReverseApplication::defaultName = ":";

bool ReverseApplication::unapplyVariables(ExpPtrArg e,
                                          ExpPtrArg l,
                                          ExpPtrArg r,
                                          Environment &env) const
{
    return proxy.unapplyVariables(e, r, l, env);
}


LowPriorityApplication::LowPriorityApplication()
    : Operator(false, 0)
{
}

ExpPtr LowPriorityApplication::operate(ExpPtrArg first,
                                       ExpPtrArg second,
                                       Environment& env) const
{
    return proxy.operate(first, second, env);
}

std::string LowPriorityApplication::show() const
{
    return LowPriorityApplication::defaultName;
}

const std::string LowPriorityApplication::defaultName = "$";

bool LowPriorityApplication::unapplyVariables(ExpPtrArg e,
                                              ExpPtrArg l,
                                              ExpPtrArg r,
                                              Environment &env) const
{
    return proxy.unapplyVariables(e, l, r, env);
}

