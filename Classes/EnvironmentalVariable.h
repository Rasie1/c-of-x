#pragma once
#include <string>
#include <memory>
#include "Expression.h"

namespace Language
{

class Expression;
class Pattern;
class Environment;

class EnvironmentalVariable
{
public:
    EnvironmentalVariable(ExpPtrArg pattern, ExpPtrArg value);
    ~EnvironmentalVariable();

    bool match(ExpPtrArg other,
               Environment* env) const;
    ExpPtr get();
    ExpPtr getPattern() const;

private:
    ExpPtr pattern;
    ExpPtr value;
};

}
