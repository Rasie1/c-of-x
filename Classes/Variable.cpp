#include "Variable.h"
#include "Environment.h"
#include "UnknownName.h"
#include "PatternAbstraction.h"

namespace Language
{

Expression* Variable::evalModifyEnv(Environment*& env)
{
    auto ret = env->get(pattern());
    if (ret == nullptr)
        return new UnknownName(name);

    return ret->eval(env);
}

Pattern* Variable::pattern()
{
    return new PatternAbstraction(name);
}

std::string Variable::toString() const
{
    return name;
}

}
