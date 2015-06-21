#include "EnvironmentalVariable.h"
#include "Pattern.h"

namespace Language
{

EnvironmentalVariable::EnvironmentalVariable(const std::shared_ptr<Pattern>&  pattern, Expression* value)
    : pattern(pattern),
      value(value)
{}

EnvironmentalVariable::~EnvironmentalVariable()
{
    //delete value; // deletes when assignment is deleted
}

bool EnvironmentalVariable::match(const std::shared_ptr<Pattern>& other,
                                  Environment* env) const
{
    return other->match(pattern, env);
}

Expression* EnvironmentalVariable::get()
{
    return value;
}

std::shared_ptr<Pattern> EnvironmentalVariable::getPattern() const
{
    return pattern;
}

}
