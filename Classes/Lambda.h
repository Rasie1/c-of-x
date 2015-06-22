#pragma once
#include "Expression.h"

namespace Language
{

class Lambda : public Expression
{
public:
    Lambda() {}

    ExpPtr apply(const ExpPtr& e, Environment*& env) const override;
    std::string toString() const override;
    static const std::string defaultName;

private:
};

class Pattern;
class LambdaArguments : public Expression
{
public:
    LambdaArguments(const PatPtr& argument);

    ExpPtr apply(const ExpPtr& e, Environment*& env) const override;
    std::string toString() const override;

    PatPtr pattern;
};

}
