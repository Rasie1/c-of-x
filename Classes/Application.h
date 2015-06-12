#pragma once
#include "Expression.h"

namespace Language
{

class Application : public Expression
{
public:
    Application(Expression* function, Expression* argument);
    ~Application();

    Expression* eval(Environment* env) override;
    Expression* evalModifyEnv(Environment*& env) override;
    Pattern* pattern() override;
    std::string toString() const override;

private:
    Expression* function;
    Expression* argument;
};

}
