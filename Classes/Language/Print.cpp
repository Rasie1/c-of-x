#include "Print.h"
#include <iostream>
#include "Environment.h"
#include "Identifier.h"

ExpPtr Print::apply(ExpPtrArg e, Environment*& env) const
{
    ExpPtr arg;
    arg = Identifier::unwrapIfId(e, env);
    auto evaluated = arg->eval(env);
    std::cout << Identifier::unwrapIfId(evaluated, env)->show();
    return evaluated;
}

std::string Print::show() const
{
    return defaultName;
}

const std::string Print::defaultName = "print";


ExpPtr PrintInfo::apply(ExpPtrArg e, Environment*& env) const
{
    ExpPtr arg;
    arg = d_cast<Identifier>(e);
    if (arg)
        arg = env->get(e);
    else
        arg = e;
    auto evaluated = arg->eval(env);
    std::cout << evaluated->show();
    return evaluated;
}

std::string PrintInfo::show() const
{
    return defaultName;
}

const std::string PrintInfo::defaultName = "info";
