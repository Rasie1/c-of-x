#pragma once
#include "Expression.h"

class Operator : public Expression
{
public:
    Operator(bool isRightAssociative = false, int priority = 5);

    virtual ExpPtr operate(
            ExpPtrArg first,
            ExpPtrArg second,
            Environment*& env) const = 0;

    bool isOperator(Environment* env) const override;
    PatPtr pattern() const override;
    bool isRightAssociative;
    int priority;
    const std::string name;

    virtual bool unwind(ExpPtr& left,
                        ExpPtr& right,
                        ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment*& env);

    virtual std::string show() const;
private:
};
/*
class CustomOperator : public Operator
{
public:
    CustomOperator(Expression* function);

    ExpPtr operate(ExpPtrArg first,
                   ExpPtrArg second,
                   Environment*& env) const override;

    std::string show() const override;
};*/

class MakeOperator : public Expression
{
public:
    ExpPtr apply(ExpPtrArg e, Environment*& env) const override;
    std::string show() const override;
    static const std::string defaultName;
};

class OperatorOperator : public Expression
{
public:
    ExpPtr apply(ExpPtrArg e, Environment*& env) const override;
    std::string show() const override;


};
