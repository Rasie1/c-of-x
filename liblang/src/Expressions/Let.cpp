#include "Expressions/Let.h"
#include "Expressions/Any.h"
#include "Expressions/Void.h"
#include "System/Environment.h"
#include "System/Cast.h"

Object Let::apply(const Object& e, Environment& env)
{
    // TODO: add new value to env unionized with old value
    auto result = e->eval(env);
    if (checkType<Void>(env, result))
        return result;
    else
        return makeObject<LetContext>(env);
}

std::string Let::show() const
{
    return defaultName;
}

const std::string Let::defaultName = "let";


LetContext::LetContext(const Environment& env) :
    env(env)
{
}

Object LetContext::apply(const Object& e, Environment& env)
{
    auto envCopy = this->env;
    auto result = e->eval(envCopy);
    return result;
}

std::string LetContext::show() const
{
    return Let::defaultName + "[...]";
}
