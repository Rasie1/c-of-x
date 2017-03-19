#include "Expressions/Print.h"
#include <iostream>
#include "System/Environment.h"
#include "Expressions/Identifier.h"
#include "Expressions/Void.h"
#include "Expressions/Any.h"
#include "System/Cast.h"

Object Print::apply(const Object& e, Environment& env)
{
    Object arg;
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


Object PrintInfo::apply(const Object& e, Environment& env)
{
    Object arg;
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


Object PrintEnv::apply(const Object& e, Environment& env)
{
    std::cout << env.show();
    return makeObject<Void>();
}

std::string PrintEnv::show() const
{
    return defaultName;
}

const std::string PrintEnv::defaultName = "printenv";
