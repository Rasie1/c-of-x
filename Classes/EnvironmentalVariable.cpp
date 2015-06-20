#include "EnvironmentalVariable.h"
#include "Pattern.h"

namespace Language
{

EnvironmentalVariable::EnvironmentalVariable(Pattern* pattern, Expression* value)
    : pattern(pattern),
      value(value)
{}

EnvironmentalVariable::~EnvironmentalVariable()
{
    delete pattern;
    //delete value; // deletes when assignment is deleted
}

bool EnvironmentalVariable::match(Pattern* other, Environment* env)
{
    return other->match(pattern, env);
}

Expression* EnvironmentalVariable::get()
{
    return value;
}

Pattern* EnvironmentalVariable::getPattern()
{
    return pattern;
}

}
