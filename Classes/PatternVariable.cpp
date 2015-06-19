#include "PatternVariable.h"
#include "Application.h"
#include "Integer.h"
#include "Environment.h"

namespace Language
{

bool PatternVariable::match(Pattern* other, Environment* env)
{
    auto x = dynamic_cast<PatternVariable*>(other);
    if (x == nullptr)
        return false;
    return x->name == name;
}


std::string PatternVariable::toString() const
{
    return name;
}

}
