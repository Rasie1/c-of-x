#include "Print.h"
#include <iostream>
#include "Environment.h"

ExpPtr Print::apply(ExpPtrArg e, Environment*& env) const
{
    std::cout << e->show() << std::endl;
    return e->eval(env);
}

std::string Print::show() const
{
    return defaultName;
}

const std::string Print::defaultName = "print";
