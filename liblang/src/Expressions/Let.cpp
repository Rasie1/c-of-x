#include "Expressions/Let.h"
#include "Expressions/Any.h"
#include "Expressions/Void.h"
#include "System/Environment.h"

ExpPtr Let::apply(ExpPtrArg e, Environment& env)
{
    // TODO: add new value to env unionized with old value
    auto result = e->eval(env);
    if (checkType<Void>(result))
        return result;
    else
        return make_ptr<LetContext>(env);
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

ExpPtr LetContext::apply(ExpPtrArg e, Environment& env)
{
    auto envCopy = this->env;
    auto result = e->eval(envCopy);
    return result;
}

std::string LetContext::show() const
{
    return Let::defaultName + "[...]";
}
