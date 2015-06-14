#include "Print.h"
#include <iostream>

namespace Language
{

Expression* Print::applyConstEnv(Expression* e, Environment* env)
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

}
