#pragma once
#include "Expression.h"
#include <list>
#include <string>
#include "Pair.h"

namespace Language
{

class List : public Expression
{
public:
    List(Pair* pair);

    std::string toString() const override;

    Expression* head();
    Expression* tail();

    Pair* pair;
};

class MakeList : public Expression
{
public:
    MakeList();

    Expression* applyConstEnv(Expression *e, Environment *env) override;
    std::string toString() const override;
};

}
