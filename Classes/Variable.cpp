#include "Variable.h"
#include "Environment.h"
#include "UnknownName.h"
#include "PatternAbstraction.h"

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
    return new PatternAbstraction(name);
}

std::string Variable::toString() const
{
    return name;
}

}
