#include "Print.h"
#include <iostream>
#include "Pattern.h"

namespace Language
{

Expression* Print::apply(Expression* e, Environment*& env)
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



Expression* PrintPattern::apply(Expression* e, Environment*& env)
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
