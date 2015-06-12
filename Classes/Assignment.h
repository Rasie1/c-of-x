#pragma once
#include "Function.h"

namespace Language
{

class Assignment : public Expression
{
public:
    Assignment();

    Expression* apply(Expression* e, Environment* env);
    std::string toString() const;
private:
};

class AssignmentOfValue : public Expression
{
public:
    AssignmentOfValue(Expression* e);
    ~AssignmentOfValue();

    Expression* apply(Expression* e, Environment* env);
    std::string toString() const;

    friend class Application;

private:
    Expression* value;
};

}
