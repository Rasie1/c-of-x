#include "Lambda.h"
#include <iostream>
#include "Closure.h"
#include "Function.h"
#include "Environment.h"
#include "Operation.h"

Lambda::Lambda()
    : Operator(true, 2)
{
}

ExpPtr Lambda::operate(ExpPtrArg first,
                       ExpPtrArg second,
                       Environment& env) const
{
    auto ret = make_ptr<Closure>(first, second, env, 1);
    return ret;
}

std::string Lambda::show() const
{
    return Lambda::defaultName;
}

const std::string Lambda::defaultName = "\\";
