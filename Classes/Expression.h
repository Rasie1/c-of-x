#pragma once
#include <string>

namespace Language
{

class Environment;

class Pattern;
class Expression
{
public:
    Expression();
    virtual ~Expression();

    virtual Expression* eval(Environment* env);
    virtual Expression* apply(Expression* e, Environment* env);
    virtual Expression* evalModifyEnv(Environment*& env);
    virtual Pattern* pattern();
    virtual std::string toString() const;
};

}
