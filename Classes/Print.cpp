#include "Print.h"
#include <iostream>
#include "Pattern.h"
#include "Environment.h"

namespace Language
{

ExpPtr Print::apply(ExpPtrArg e, Environment*& env) const
{
    //auto newEnv = env;
    //auto evaluated = e->eval(newEnv);
    std::cout << e->show() << std::endl;
    return e->eval(env);
}

std::string Print::show() const
{
    return defaultName;
}

const std::string Print::defaultName = "print";


ExpPtr Show::apply(ExpPtrArg e, Environment*& env) const
{
    std::cout << e->show() << std::endl;
    return e->eval(env);
}

std::string Show::show() const
{
    return defaultName;
}

const std::string Show::defaultName = "show";



ExpPtr PrintPattern::apply(ExpPtrArg e, Environment*& env) const
{
    std::cout << e->pattern()->show() << std::endl;

    auto newEnv = env;
    return e->eval(newEnv);
}

std::string PrintPattern::show() const
{
    return defaultName;
}

const std::string PrintPattern::defaultName = "printPattern";


}
