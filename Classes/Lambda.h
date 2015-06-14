#pragma once
#include "Expression.h"

namespace Language
{

class Lambda : public Expression
{
public:
    Lambda() {}

    Expression* apply(Expression* e, Environment* env) override;
    std::string toString() const override;
    static const std::string defaultName;

private:
};

class Pattern;
class LambdaArguments : public Expression
{
public:
    LambdaArguments(Pattern* argument);

    Expression* apply(Expression* e, Environment* env) override;
    std::string toString() const override;

    Pattern* pattern;
};

}
