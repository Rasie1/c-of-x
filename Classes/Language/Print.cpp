#include "Print.h"
#include <iostream>
#include "Environment.h"
#include "Identifier.h"

ExpPtr Print::apply(ExpPtrArg e, Environment*& env) const
{
    ExpPtr arg;
    arg = d_cast<Identifier>(e);
    if (arg)
        arg = env->getEqual(e);
    else
        arg = e;
    auto evaluated = arg->eval(env);
    std::cout << evaluated->show();
    return evaluated;
}

std::string Print::show() const
{
    return defaultName;
}

const std::string Print::defaultName = "print";
