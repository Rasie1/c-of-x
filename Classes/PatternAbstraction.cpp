#include "PatternAbstraction.h"
#include "Application.h"
#include "Integer.h"
#include "Environment.h"

namespace Language
{

bool PatternAbstraction::match(Pattern* other, Environment* env)
{
    auto x = dynamic_cast<PatternAbstraction*>(other);
    if (x == nullptr)
        return false;
    return x->name == name;
}


std::string PatternAbstraction::toString() const
{
    return name;
}

}
