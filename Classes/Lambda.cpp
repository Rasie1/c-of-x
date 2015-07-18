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

ExpPtr Lambda::apply(const ExpPtr& e, Environment*& env) const
{
    return e->pattern();//std::make_shared<LambdaArguments>(e->pattern());
}

std::string Lambda::toString() const
{
    return defaultName;
}

const std::string Lambda::defaultName = "\\";


LambdaArguments::LambdaArguments(const PatPtr& argument)
    : pattern(argument)
{

}

ExpPtr LambdaArguments::apply(const ExpPtr& e, Environment*& env) const
{
    auto ret = std::make_shared<Closure>(std::make_shared<Function>(e, pattern),
                                         env);
    return ret;
}

std::string LambdaArguments::toString() const
{
    return "\u03BB(" + pattern->toString() + ")";
}

ExpPtr Lambda::construct(PatPtr arg, ExpPtr body, Environment* env)
{
    /*auto lambdaApp = std::make_shared<Operation>(
                std::make_shared<Application>(),
                std::make_shared<Lambda>(),
                std::make_shared<LambdaArguments>(arg));
    auto lambda = std::make_shared<Operation>(
                std::make_shared<ClosureOperator>(),
                arg,
                body);
    return lambda->eval(env);*/
    auto lambda = std::make_shared<Operation>(
                std::make_shared<Application>(),
                std::make_shared<LambdaArguments>(arg),
                body);
    return lambda->eval(env);
    //return std::make_shared<Closure>(std::make_shared<Function>(body,
    //                                                            arg),
    //                                 env);
}


ClosureOperator::ClosureOperator()
    : Operator(false, 9)
{
}

ExpPtr ClosureOperator::operate(const ExpPtr& first,
                                const ExpPtr& second,
                                Environment*& env) const
{
    return Lambda::construct(std::static_pointer_cast<Pattern>(first->eval(env)),
                             second,
                             env);
    //auto fun = std::make_shared<Function>(second,
    //                                      std::static_pointer_cast<Pattern>(first));
    //return std::make_shared<Closure>(fun, env);
}

std::string ClosureOperator::toString() const
{
    return ".";
}

bool ClosureOperator::unwind(const ExpPtr& left,
                             const ExpPtr& right,
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
