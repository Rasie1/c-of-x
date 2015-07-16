#pragma once
#include <string>
#include <memory>

namespace Language
{

class Expression;
class Pattern;
typedef std::shared_ptr<Expression> ExpPtr;
typedef std::shared_ptr<Pattern>    PatPtr;
class Environment;

class EnvironmentalVariable
{
public:
    EnvironmentalVariable(const PatPtr& pattern, const ExpPtr& value);
    ~EnvironmentalVariable();

    bool match(const PatPtr& other,
               Environment* env) const;
    ExpPtr get();
    PatPtr getPattern() const;

private:
    PatPtr pattern;
    ExpPtr value;
};

}
