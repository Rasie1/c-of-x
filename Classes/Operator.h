#pragma once
#include <string>
#include "Expression.h"

namespace Language
{

class Operator : public Expression
{
public:
    Operator(bool isRightAssociative = false);

    virtual Expression* operate(Expression* first,
                                Expression* second,
                                Environment*& env) = 0;
    bool isRightAssociative;
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
