#pragma once
#include "Expression.h"

namespace Language
{

class Function;
class Environment;
class Closure : public Expression
{
public:
    Closure(Function* function, Environment* env);
    ~Closure();

    Expression* applyConstEnv(Expression* e, Environment* env) override;
    std::string toString() const override;

private:
    Function* function;
    Environment* env;
};

}
