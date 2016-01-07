#include "EnvironmentalVariable.h"

EnvironmentalVariable::EnvironmentalVariable(ExpPtrArg  pattern, ExpPtrArg value)
    : pattern(pattern),
      value(value)
{}

EnvironmentalVariable::~EnvironmentalVariable()
{
    //delete value; // deletes when assignment is deleted
}

bool EnvironmentalVariable::match(ExpPtrArg other,
                                  Environment* env) const
{
    return other->match(pattern);
}

ExpPtr EnvironmentalVariable::get()
{
    return value;
}

ExpPtr EnvironmentalVariable::getPattern() const
{
    return pattern;
}
