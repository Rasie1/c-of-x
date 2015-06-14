#pragma once
#include "Function.h"
#include "Integer.h"

namespace Language
{

class AdditionOfValue : public Expression
{
public:
    AdditionOfValue(long long value = 0)
        : value(value) {}

    Expression* applyConstEnv(Expression* e, Environment* env) override;
    std::string toString() const override;

private:
    long long value;
};

class Addition : public Expression
{
public:
    Addition() {}

    Expression* applyConstEnv(Expression* e, Environment* env) override;
    std::string toString() const override;
    static const std::string defaultName;
};

}
