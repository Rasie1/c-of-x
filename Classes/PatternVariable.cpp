#include "PatternVariable.h"
#include "PatternOperator.h"
#include "Application.h"
#include "Integer.h"
#include "Environment.h"

namespace Language
{

bool PatternVariable::match(Pattern* other, Environment* env)
{

    auto y = dynamic_cast<PatternVariable*>(other);
    if (y)
        return y->name == name;
    auto x = dynamic_cast<PatternOperator*>(other);
    if (x == nullptr)
        return false;
    return x->name == name;
    /*
    auto x = dynamic_cast<PatternVariable*>(other);
    if (x == nullptr)
        return false;
    return x->name == name;*/
}


std::string PatternVariable::toString() const
{
    return name;
}

}
