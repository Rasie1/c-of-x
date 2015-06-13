#pragma once
#include "Expression.h"
#include "Function.h"
#include <string>

namespace Language
{

class Pair : public Expression
{
public:
    Pair(Expression* l, Expression* r)
        : l(l),
          r(r) {}

    Expression* eval(Environment* env) override;
    std::string toString() const;

    Expression* l;
    Expression* r;
};

class PairConstructorWithValue : public Expression
{
public:
    PairConstructorWithValue(Expression* e)
        : value(e) {}

    Expression* eval(Environment* env) override;
    Expression* apply(Expression* e, Environment* env) override;
    std::string toString() const override;

    Expression* value;
};

class PairConstructor : public Expression
{
public:
    PairConstructor() {}

    Expression* eval(Environment* env) override;
    Expression* apply(Expression* e, Environment* env) override;
    std::string toString() const override;
    static const std::string defaultName;
};

}
