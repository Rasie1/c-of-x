#include "Print.h"
#include <iostream>

namespace Language
{

Expression* Print::apply(Expression* e, Environment* env)
{
    auto evaluated = e->eval(env);
    std::cout << evaluated->toString() << std::endl;
    return evaluated;
}

std::string Print::toString() const
{
    return defaultName;
}

const std::string Print::defaultName = "print";

}
