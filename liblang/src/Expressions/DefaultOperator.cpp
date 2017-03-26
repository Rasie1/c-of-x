#include "Expressions/DefaultOperator.h"
#include "Expressions/Operation.h"
#include "Expressions/Identifier.h"
#include "Expressions/Void.h"
#include "System/Environment.h"
#include "System/Cast.h"

DefaultOperator::DefaultOperator()
    : Operator(false, 4, true)
{
}

Object DefaultOperator::operate(const Object& first,
                                const Object& second,
                                Environment& env)
{
    Object l, r;
    {
        if (auto o = cast<Operation>(env, first))
        {
            if (auto d = cast<DefaultOperator>(env, o->op))
                l = env.defaultOperator->operate(o->left, o->right, env);
        }
        else
            l = first;
    }
    {
        if (auto o = cast<Operation>(env, second))
        {
            if (auto d = cast<DefaultOperator>(env, o->op))
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
