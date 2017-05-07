#include "Expressions/Lambda.h"
#include <iostream>
#include "Expressions/Closure.h"
#include "Expressions/Morphism.h"
#include "Expressions/Identifier.h"
#include "Expressions/Operation.h"
#include "System/Environment.h"
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
    if (auto id = cast<Identifier>(env, first))
        newEnv.erase(id->name);
    auto arg = first->eval(newEnv); // not really good. Should detect bound variables
     //                                and erase them
    auto body = second;
    auto ret = makeObject<Closure>(arg, body, newEnv);
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
