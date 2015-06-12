#include "UnknownName.h"
#include "Pattern.h"

namespace Language
{

Expression* UnknownName::eval(Environment* env)
{
    return this;
}

Pattern* UnknownName::pattern()
{
    return new PatternAbstraction(name);
}

std::string UnknownName::toString() const
{
    return "Identifier not found: " + name;
}

}
