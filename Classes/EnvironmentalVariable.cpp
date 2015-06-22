#include "EnvironmentalVariable.h"
#include "Pattern.h"

namespace Language
{

EnvironmentalVariable::EnvironmentalVariable(const PatPtr&  pattern, const ExpPtr& value)
    : pattern(pattern),
      value(value)
{}

EnvironmentalVariable::~EnvironmentalVariable()
{
    //delete value; // deletes when assignment is deleted
}

bool EnvironmentalVariable::match(const PatPtr& other,
                                  Environment* env) const
{
    return other->match(pattern, env);
}

ExpPtr EnvironmentalVariable::get()
{
    return value;
}

PatPtr EnvironmentalVariable::getPattern() const
{
    return pattern;
}

}
