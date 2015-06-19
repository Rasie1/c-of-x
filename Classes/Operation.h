#pragma once
#include "Expression.h"

namespace Language
{

class Operator;
class Operation : public Expression
{
public:
    Operation(Operator* op, Expression* left, Expression* right);
    ~Operation();

    Expression* eval(Environment*& env) override;
    Pattern* pattern() override;
    std::string toString() const override;
private:
    Operator* op;
    Expression* left;
    Expression* right;
};
/*
class MakeOperator : public Expression
{
    Expression* apply(Expression* e, Environment*& env) override;
    std::string toString() const override;
    static const std::string defaultName;

private:
};*/

}
