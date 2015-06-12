#pragma once
#include "Expression.h"
#include <string>

namespace Language
{

class Pattern : public Expression
{
public:
    Pattern();

    virtual bool match(Pattern* other, Environment* env);
    std::string toString() const override;
};

class PatternAbstraction : public Pattern
{
public:
    PatternAbstraction(const std::string& name)
        : name(name) {}

    bool match(Pattern* other, Environment* env) override;
    std::string toString() const override;

    std::string name;
};

class PatternApplication : public Pattern
{
public:
    PatternApplication(Pattern* body, Pattern* argument);

    bool match(Pattern* other, Environment* env) override;
    std::string toString() const override;

    Pattern* body;
    Pattern* argument;
};

class PatternPredicate : public Pattern
{
    PatternPredicate(Expression* predicate);

    bool match(Pattern* other, Environment* env) override;
    std::string toString() const override;

    Expression* predicate;
};

}
