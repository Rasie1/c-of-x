#include "UnknownName.h"
#include "PatternVariable.h"

namespace Language
{

Expression* UnknownName::evalConstEnv(Environment* env)
{
    return this;
}

Pattern* UnknownName::pattern()
{
    return new PatternVariable(name);
}

std::string UnknownName::toString() const
{
    return "UnknownName(" + name + ")";
}

}
