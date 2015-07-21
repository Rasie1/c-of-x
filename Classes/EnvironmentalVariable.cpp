#include "EnvironmentalVariable.h"
#include "Pattern.h"

namespace Language
{

EnvironmentalVariable::EnvironmentalVariable(const ExpPtr&  pattern, const ExpPtr& value)
    : pattern(pattern),
      value(value)
{}

EnvironmentalVariable::~EnvironmentalVariable()
{
    //delete value; // deletes when assignment is deleted
}

bool EnvironmentalVariable::match(const ExpPtr& other,
                                  Environment* env) const
{
    return other->match(pattern, env);
}

ExpPtr EnvironmentalVariable::get()
{
    return value;
}

ExpPtr EnvironmentalVariable::getPattern() const
{
    return pattern;
}

}
