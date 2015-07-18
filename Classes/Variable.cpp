#include "Variable.h"
#include "Environment.h"
#include "UnknownName.h"
#include "PatternVariable.h"
#include <string>

namespace Language
{

ExpPtr Variable::evaluation(Environment*& env) const
{
    auto ret = env->get(pattern());
    if (ret == nullptr)
        return std::make_shared<UnknownName>(name);

    auto newEnv = env;
    return ret->eval(newEnv);
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
    return "$" + name;
}

bool Variable::unwind(ExpPtr& lvalue,
                      ExpPtr& rvalue,
                      Environment*& env)
{
    auto evaluated = env->get(rvalue->pattern());
    if (evaluated == nullptr)
        return false;
    rvalue = evaluated;

    return true;
}

}
