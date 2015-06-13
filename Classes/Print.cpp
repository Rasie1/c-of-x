#include "Print.h"
#include <iostream>

namespace Language
{

Expression* Print::apply(Expression* e, Environment* env)
{
    std::cout << e->eval(env)->toString() << std::endl;
    return e;
}

std::string Print::toString() const
{
    return defaultName;
}

const std::string Print::defaultName = "print";

}
