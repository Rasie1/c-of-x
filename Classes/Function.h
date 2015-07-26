#pragma once
#include "Expression.h"

namespace Language
{

class Function : public Expression
{
public:
    Function(const ExpPtr& body,
             const ExpPtr& argument);
    ~Function();

    ExpPtr evaluation(Environment*& env) const override;
    ExpPtr apply(const ExpPtr& e, Environment*& env) const override;
    std::string toString() const override;

    ExpPtr body;
    ExpPtr argument;
    std::string name;
};

}
