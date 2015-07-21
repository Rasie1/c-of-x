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
    EnvironmentalVariable(const ExpPtr& pattern, const ExpPtr& value);
    ~EnvironmentalVariable();

    bool match(const ExpPtr& other,
               Environment* env) const;
    ExpPtr get();
    ExpPtr getPattern() const;

private:
    ExpPtr pattern;
    ExpPtr value;
};

}
