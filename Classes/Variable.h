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
    bool match(const ExpPtr& other, Environment* env) const;
    bool isOperator(Environment* env) const override;
    std::string toString() const override;

    virtual bool unwind(ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment*& env) override;

    bool hasNonOpVariable(Environment* env) const;
    void getAllVariables(
            std::vector<std::shared_ptr<Variable>>& variables) override;

    std::string name;
};

}
