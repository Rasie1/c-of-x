#pragma once
#include "Expression.h"

namespace Language
{

class Operator : public Expression
{
public:
    Operator(bool isRightAssociative = false, int priority = 5);

    virtual ExpPtr operate(
            const ExpPtr& first,
            const ExpPtr& second,
            Environment*& env) const = 0;

    bool isOperator(Environment* env) const override;
    PatPtr pattern() const override;
    virtual PatPtr leftPattern(const ExpPtr& e) const;
    virtual PatPtr rightPattern(const ExpPtr& e) const;
    bool isRightAssociative;
    int priority;
    const std::string name;

    virtual std::string toString() const;
private:
};
/*
class CustomOperator : public Operator
{
public:
    CustomOperator(Expression* function);

    Expression* function;
};

class PrecedenceInfo// : public Expression
{
public:
    PrecedenceInfo(Operator* more, Operator* less);

    std::string toString() const override;

    Operator* more;
    Operator* less;
};*/

}
