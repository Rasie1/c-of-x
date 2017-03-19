#include "Expressions/Lambda.h"
#include <iostream>
#include "Expressions/Closure.h"
#include "Expressions/Morphism.h"
#include "System/Environment.h"
#include "Expressions/Operation.h"
#include "System/Cast.h"

Lambda::Lambda()
    : Operator(true, 2, true)
{
}

Object Lambda::operate(const Object& first,
                       const Object& second,
                       Environment& env)
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

const std::string Lambda::defaultName = "=>";

// bool Lambda::unapplyVariables(const Object& e,
//                               const Object& l,
//                               const Object& r,
//                               Environment &env) const
// {
    
// }
