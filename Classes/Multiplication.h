#pragma once
#include "Function.h"
#include "Integer.h"

namespace Language
{

class MultiplicationOfValue : public Expression
{
public:
    MultiplicationOfValue(long long value = 0)
        : value(value) {}

    Expression* apply(Expression* e, Environment* env) override;
    std::string toString() const override;

private:
    long long value;
};

class Multiplication : public Expression
{
public:
    Multiplication() {}

    Expression* apply(Expression* e, Environment* env) override;
    std::string toString() const override;
    static const std::string defaultName;
};

}
