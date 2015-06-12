#pragma once
#include "Function.h"
#include "Integer.h"

namespace Language
{

class SubtractionOfValue : public Expression
{
public:
    SubtractionOfValue(long long value = 0)
        : value(value) {}

    Expression* apply(Expression* e, Environment* env) override;
    std::string toString() const override;

private:
    long long value;
};

class Subtraction : public Expression
{
public:
    Subtraction() {}

    Expression* apply(Expression* e, Environment* env) override;
    std::string toString() const override;
};

}
