#pragma once
#include "Expression.h"
#include "Operator.h"

namespace Language
{

class Lambda : public Expression
{
public:
    Lambda() {}

    ExpPtr apply(const ExpPtr& e, Environment*& env) const override;
    std::string toString() const override;
    static const std::string defaultName;

    // Convenience function for fast lambda creation in C++
    static ExpPtr construct(ExpPtr arg, ExpPtr body, Environment* c);
private:
};

class Pattern;
class LambdaArguments : public Expression
{
public:
    LambdaArguments(const ExpPtr& argument);

    ExpPtr apply(const ExpPtr& e, Environment*& env) const override;
    std::string toString() const override;

    ExpPtr pattern;
};

class ClosureOperator : public Operator
{
public:
    ClosureOperator();

    ExpPtr operate(const ExpPtr& first,
                   const ExpPtr& second,
                   Environment*& env) const override;

    virtual bool unwind(ExpPtr& left,
                        ExpPtr& right,
                        ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment*& env) override;

    std::string toString() const override;

    static const std::string defaultName;
private:
    Environment* environment;
};

}
