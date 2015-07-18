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

    virtual bool unwind(const ExpPtr& left,
                        const ExpPtr& right,
                        ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment*& env);

    virtual std::string toString() const;
private:
};


class MakeOperator : public Expression
{
public:
    ExpPtr apply(const ExpPtr& e, Environment*& env) const override;
    std::string toString() const override;
    static const std::string defaultName;
};

class OperatorOperator : public Expression
{
public:
    ExpPtr apply(const ExpPtr& e, Environment*& env) const override;
    std::string toString() const override;


};


}
