#pragma once
#include "Expression.h"

namespace Language
{

class Mutation : public Expression
{
public:
    Mutation();

    Expression* apply(Expression* e, Environment* env);
    std::string toString() const;
    static const std::string defaultName;
private:
};

class MutationWithValue : public Expression
{
public:
    MutationWithValue(Expression* e);
    ~MutationWithValue();

    Expression* apply(Expression* e, Environment* env);
    std::string toString() const;

private:
    Expression* value;
};

}
