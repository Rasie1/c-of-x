#pragma once
#include "Expression.h"

namespace Language
{

class Function : public Expression
{
public:
    Function(Expression* body, Pattern* argument);
    ~Function();

    Expression* evalConstEnv(Environment* env) override;
    Expression* applyConstEnv(Expression* e, Environment* env) override;
    std::string toString() const override;

    Expression* body;
    Pattern* argument;
};

class PrintPattern : public Expression
{
public:
    Expression* applyConstEnv(Expression* e, Environment* env) override;
    std::string toString() const override;
    static const std::string defaultName;
};

class Preprocess : public Expression
{
public:
    Expression* applyConstEnv(Expression* e, Environment* env) override;
    std::string toString() const override;
};

}
