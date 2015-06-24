#include "Print.h"
#include <iostream>
#include "Pattern.h"
#include "Environment.h"

namespace Language
{

ExpPtr Print::apply(const ExpPtr& e, Environment*& env) const
{
    auto newEnv = env;
    auto evaluated = e->eval(newEnv);
    std::cout << evaluated->toString() << std::endl;
    return evaluated;
}

std::string Print::toString() const
{
    return defaultName;
}

const std::string Print::defaultName = "print";



ExpPtr PrintPattern::apply(const ExpPtr& e, Environment*& env) const
{
    std::cout << e->pattern()->toString() << std::endl;

    auto newEnv = env;
    return e->eval(newEnv);
}

std::string PrintPattern::toString() const
{
    return defaultName;
}

const std::string PrintPattern::defaultName = "printPattern";


}
