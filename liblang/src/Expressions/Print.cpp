#include "Expressions/Print.h"
#include <iostream>
#include "System/Environment.h"
#include "Expressions/Identifier.h"
#include "Expressions/Void.h"
#include "Expressions/Any.h"
#include "Expressions/Integer.h"
#include "System/Cast.h"

Object Print::apply(const Object& e, Environment& env)
{
    Object arg;
    arg = Identifier::unwrapIfId(e, env);
    auto evaluated = arg;

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

Object CastToInt::apply(const Object& e, Environment& env)
{
    auto ret = cast<Integer>(env, e);
    return Object(ret);
}

std::string CastToInt::show() const
{
    return defaultName;
}

const std::string CastToInt::defaultName = "toInt";

Object PrintInfo::apply(const Object& e, Environment& env)
{
    Object arg = checkType<Identifier>(env, e) ? 
        env.get(cast<Identifier>(env, e)->name) : e;
    auto evaluated = arg;
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
