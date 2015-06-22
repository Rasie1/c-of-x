#pragma once
#include "Expression.h"

namespace Language
{

class Function : public Expression
{
public:
    Function(const ExpPtr& body,
             const PatPtr& argument);
    ~Function();

    ExpPtr eval(Environment*& env) const override;
    ExpPtr apply(const ExpPtr& e, Environment*& env) const override;
    std::string toString() const override;

    ExpPtr body;
    PatPtr argument;
};

}
