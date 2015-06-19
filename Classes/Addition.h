#pragma once
#include "Function.h"
#include "Integer.h"
#include "Operator.h"

namespace Language
{

class AdditionOfValue : public Expression
{
public:
    AdditionOfValue(long long value = 0)
        : value(value) {}

    Expression* apply(Expression* e, Environment*& env) override;
    std::string toString() const override;

private:
    long long value;
};

class Addition : public Expression
{
public:
    Addition() {}

    Expression* apply(Expression* e, Environment*& env) override;
    std::string toString() const override;
    static const std::string defaultName;
};

class Plus : public Operator
{
public:
    Expression* operate(Expression* first,
                        Expression* second,
                        Environment*& env) override;
    std::string toString() const override;

    static const std::string defaultName;
};

}
