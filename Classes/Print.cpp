#include "Print.h"
#include <iostream>
#include "Environment.h"

ExpPtr Print::apply(ExpPtrArg e, Environment*& env) const
{
    auto evaluated = e->eval(env);
    std::cout << evaluated->show() << std::endl;
    return evaluated;
}

std::string Print::show() const
{
    return defaultName;
}

const std::string Print::defaultName = "print";
