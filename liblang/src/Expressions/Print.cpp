#include "Expressions/Print.h"
#include <iostream>
#include "System/Environment.h"
#include "Expressions/Identifier.h"
#include "Expressions/Void.h"
#include "Expressions/Any.h"

ExpPtr Print::apply(ExpPtrArg e, Environment& env) const
{
    ExpPtr arg;
    arg = Identifier::unwrapIfId(e, env);
    auto evaluated = arg->eval(env);

    env.debugPrint("stdout: [", false);
    std::cout << Identifier::unwrapIfId(evaluated, env)->show();
    env.debugPrint("]\n", false);
    return evaluated;
}

std::string Print::show() const
{
    return defaultName;
}

const std::string Print::defaultName = "print";


ExpPtr PrintInfo::apply(ExpPtrArg e, Environment& env) const
{
    ExpPtr arg;
    arg = d_cast<Identifier>(e);
    if (arg)
        arg = env.get(e);
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


ExpPtr PrintEnv::apply(ExpPtrArg e, Environment& env) const
{
    std::cout << env.show();
    return make_ptr<Void>();
}

std::string PrintEnv::show() const
{
    return defaultName;
}

const std::string PrintEnv::defaultName = "printenv";
