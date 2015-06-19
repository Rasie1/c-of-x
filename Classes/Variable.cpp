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

Pattern* Variable::pattern()
{
    return new PatternVariable(name);
}

std::string Variable::toString() const
{
    return name;
}

}
