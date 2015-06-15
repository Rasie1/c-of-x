#pragma once
#include "Function.h"

namespace Language
{

class Assignment : public Expression
{
public:
    Assignment();

    Expression* applyConstEnv(Expression* e, Environment* env) override;
    std::string toString() const;
    static const std::string defaultName;
private:
};

class AssignmentOfValue : public Expression
{
public:
    AssignmentOfValue(Expression* e);
    ~AssignmentOfValue();

    Expression* applyConstEnv(Expression* e, Environment* env) override;
    Expression* apply(Expression* e, Environment*& env) override;
    std::string toString() const;

    friend class Application;

private:
    Expression* value;
};

}
