#pragma once
#include <string>
#include <memory>
#include "Expression.h"

class Expression;
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
