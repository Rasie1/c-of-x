#include "Variable.h"
#include "Environment.h"
#include "UnknownName.h"
#include "PatternVariable.h"

namespace Language
{

Expression* Variable::eval(Environment*& env)
{
    auto ret = env->get(pattern());
    if (ret == nullptr)
        return new UnknownName(name);

    return ret->evalConstEnv(env);
}

std::shared_ptr<Pattern> Variable::pattern() const
{
    return std::make_shared<PatternVariable>(name);
}

bool Variable::isOperator(Environment* env)
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
