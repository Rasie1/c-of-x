#include "Expressions/DefaultOperator.h"
#include "Expressions/Operation.h"
#include "Expressions/Identifier.h"
#include "Expressions/Void.h"
#include "System/Environment.h"

DefaultOperator::DefaultOperator()
    : Operator(false, 4, true)
{
}

ExpPtr DefaultOperator::operate(ExpPtrArg first,
                                ExpPtrArg second,
                                Environment& env)
{
    ExpPtr l, r;
    {
        if (auto o = d_cast<Operation>(first))
        {
            if (auto d = d_cast<DefaultOperator>(o->op))
                l = env.defaultOperator->operate(o->left, o->right, env);
        }
        else
            l = first;
    }
    {
        if (auto o = d_cast<Operation>(second))
        {
            if (auto d = d_cast<DefaultOperator>(o->op))
                r = env.defaultOperator->operate(o->left, o->right, env);
        }
        else
            r = second;
    }

    return env.defaultOperator->operate(l, r, env);
}

std::string DefaultOperator::show() const
{
    return DefaultOperator::defaultName;
}

const std::string DefaultOperator::defaultName = ",";
