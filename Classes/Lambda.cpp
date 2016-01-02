#include "Lambda.h"
#include <iostream>
#include "UnknownName.h"
#include "Pattern.h"
#include "Closure.h"
#include "Function.h"
#include "Environment.h"
#include "Operation.h"
#include "Application.h"
#include "Environment.h"

namespace Language
{

ExpPtr Lambda::apply(ExpPtrArg e, Environment*& env) const
{
    return e->pattern();
}

std::string Lambda::show() const
{
    return defaultName;
}

const std::string Lambda::defaultName = "\\";


LambdaArguments::LambdaArguments(ExpPtrArg argument)
    : pattern(argument)
{

}

ExpPtr LambdaArguments::apply(ExpPtrArg e, Environment*& env) const
{
    auto ret = make_ptr<Closure>(make_ptr<Function>(e, pattern),
                                 env, 1);
    return ret;
}

std::string LambdaArguments::show() const
{
    return "\u03BB(" + pattern->show() + ")";
}

ExpPtr Lambda::construct(ExpPtr arg, ExpPtr body, Environment* env)
{
    return make_ptr<Closure>(make_ptr<Function>(body,
                                                arg),
                             env, 1);
}

ClosureOperator::ClosureOperator()
    : Operator(true, 9)
{
}

ExpPtr ClosureOperator::operate(ExpPtrArg first,
                                ExpPtrArg second,
                                Environment*& env) const
{
    return Lambda::construct(first,
                             second,
                             env);
    //auto fun = make_ptr<Function>(second,
    //                                      s_cast<Pattern>(first));
    //return make_ptr<Closure>(fun, env);
}

std::string ClosureOperator::show() const
{
    return ".";
}

bool ClosureOperator::unwind(ExpPtr& left,
                             ExpPtr& right,
                             ExpPtr& lvalue,
                             ExpPtr& rvalue,
                             Environment*& env)
{
    //lvalue = left;
    //rvalue = Lambda::construct(right->pattern(),
    //                           rvalue,
    //                           env);
    return false;
}

const std::string ClosureOperator::defaultName = ".";

}
