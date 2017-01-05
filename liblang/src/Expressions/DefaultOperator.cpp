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
                                Environment& env) const
{
    ExpPtr l = (d_cast<Operation>(first) && checkType<DefaultOperator>(s_cast<Operation>(first)->op))
            ? env.defaultOperator->operate(s_cast<Operation>(first)->left,
                                           s_cast<Operation>(first)->right,
                                           env)
            : first;
    ExpPtr r = (d_cast<Operation>(second) && checkType<DefaultOperator>(s_cast<Operation>(second)->op))
            ? env.defaultOperator->operate(s_cast<Operation>(second)->left,
                                           s_cast<Operation>(second)->right,
                                           env)
            : second;
    return env.defaultOperator->operate(l, r, env);
}

std::string DefaultOperator::show() const
{
    return DefaultOperator::defaultName;
}

const std::string DefaultOperator::defaultName = ",";
