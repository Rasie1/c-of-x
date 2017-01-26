#include "Expressions/Lambda.h"
#include <iostream>
#include "Expressions/Closure.h"
#include "Expressions/Morphism.h"
#include "System/Environment.h"
#include "Expressions/Operation.h"

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

const std::string Lambda::defaultName = "\\"; // later: "=>"

// bool Lambda::unapplyVariables(ExpPtrArg e,
//                               ExpPtrArg l,
//                               ExpPtrArg r,
//                               Environment &env) const
// {
    
// }
