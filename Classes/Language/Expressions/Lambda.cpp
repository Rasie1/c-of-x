#include "Lambda.h"
#include <iostream>
#include "Closure.h"
#include "Function.h"
#include "Environment.h"
#include "Operation.h"

Lambda::Lambda()
    : Operator(true, 2, true)
{
}

ExpPtr Lambda::operate(ExpPtrArg first,
                       ExpPtrArg second,
                       Environment& env) const
{
    auto newEnv = env;
    auto arg = first->eval(newEnv);
    newEnv.erase(arg);
    arg = first->eval(newEnv); // not really good. Should detect bound variables
                               // and erase them
    auto body = second;
    auto ret = make_ptr<Closure>(arg, body, newEnv);
    return ret;
}

std::string Lambda::show() const
{
    return Lambda::defaultName;
}

const std::string Lambda::defaultName = "\\"; // later: "->"
