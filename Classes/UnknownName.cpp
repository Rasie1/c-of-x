#include "UnknownName.h"
#include "PatternVariable.h"

namespace Language
{

Expression* UnknownName::evalConstEnv(Environment* env)
{
    return this;
}

std::shared_ptr<Pattern> UnknownName::pattern() const
{
    return std::make_shared<PatternVariable>(name);
}

std::string UnknownName::toString() const
{
    return "UnknownName(" + name + ")";
}

}
