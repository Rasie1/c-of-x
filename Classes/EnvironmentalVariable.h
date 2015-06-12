#pragma once
#include <string>

namespace Language
{

class Expression;
class Pattern;
class Environment;
class EnvironmentalVariable
{
public:
    EnvironmentalVariable(Pattern* pattern, Expression* value);
    ~EnvironmentalVariable();

    bool match(Pattern* other, Environment* env);
    Expression* get();

private:
    Pattern* pattern;
    Expression* value;
};

}
