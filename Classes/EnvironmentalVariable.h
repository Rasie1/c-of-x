#pragma once
#include <string>
#include <memory>

namespace Language
{

class Expression;
class Pattern;
class Environment;
class EnvironmentalVariable
{
public:
    EnvironmentalVariable(const std::shared_ptr<Pattern>&  pattern, Expression* value);
    ~EnvironmentalVariable();

    bool match(const std::shared_ptr<Pattern>& other,
               Environment* env) const;
    Expression* get();
    std::shared_ptr<Pattern> getPattern() const;

private:
    std::shared_ptr<Pattern> pattern;
    Expression* value;
};

}
