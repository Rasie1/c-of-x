#include "Print.h"
#include <iostream>
#include "Pattern.h"

namespace Language
{

ExpPtr Print::apply(const ExpPtr& e, Environment*& env) const
{
    auto evaluated = e->evalConstEnv(env);
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
    std::cout << e->pattern()->toString();

    return e->evalConstEnv(env);
}

std::string PrintPattern::toString() const
{
    return defaultName;
}

const std::string PrintPattern::defaultName = "printPattern";


}
