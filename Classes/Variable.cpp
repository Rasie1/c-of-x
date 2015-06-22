#include "Variable.h"
#include "Environment.h"
#include "UnknownName.h"
#include "PatternVariable.h"

namespace Language
{

ExpPtr Variable::eval(Environment*& env) const
{
    auto ret = env->get(pattern());
    if (ret == nullptr)
        return std::make_shared<UnknownName>(name);

    return ret->evalConstEnv(env);
}

PatPtr Variable::pattern() const
{
    return std::make_shared<PatternVariable>(name);
}

bool Variable::isOperator(Environment* env) const
{
    auto e = env->get(pattern());
    if (e)
        return e->isOperator(env);
    else
        return false;
}

std::string Variable::toString() const
{
    return name;
}



}
