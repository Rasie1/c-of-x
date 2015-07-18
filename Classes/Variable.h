#pragma once
#include "Expression.h"
#include <string>

namespace Language
{

class Variable : public Expression
{
public:
    Variable(const std::string& name)
        : name(name) {}

    ExpPtr evaluation(Environment*& env) const override;
    PatPtr pattern() const override;
    bool isOperator(Environment* env) const override;
    std::string toString() const override;

    virtual bool unwind(ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment*& env) override;

    std::string name;
};

}
