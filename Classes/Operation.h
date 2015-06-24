#pragma once
#include "Expression.h"

namespace Language
{

class Operator;
class Operation : public Expression
{
public:
    Operation(const std::shared_ptr<Operator>& op,
              const ExpPtr& left,
              const ExpPtr& right);
    ~Operation();

    ExpPtr evaluation(Environment*& env) const override;
    PatPtr pattern() const override;
    std::string toString() const override;
private:
    std::shared_ptr<Operator> op;
    ExpPtr left;
    ExpPtr right;
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
