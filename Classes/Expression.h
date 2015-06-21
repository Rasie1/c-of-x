#pragma once
#include <string>
#include <memory>
#include "DebugHelpers.h"

namespace Language
{

class Environment;

class Pattern;
class Expression : public std::enable_shared_from_this<Expression>
{
public:
    Expression();
    virtual ~Expression();

    virtual Expression* evalConstEnv(Environment* env);
    virtual Expression* eval(Environment*& env);
    virtual Expression* applyConstEnv(Expression* e, Environment* env);
    virtual Expression* apply(Expression* e, Environment*& env);
    virtual std::shared_ptr<Pattern> pattern() const;
    virtual bool isOperator(Environment* env);
    virtual std::string toString() const;
};

}
