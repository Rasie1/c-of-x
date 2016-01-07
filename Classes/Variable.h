#pragma once
#include "Expression.h"
#include <string>

class Variable : public Expression
{
public:
    Variable(const std::string& name)
        : name(name) {}

    ExpPtr eval(Environment*& env) const override;
    bool match(ExpPtrArg other) const;
    bool isOperator(Environment* env) const override;
    std::string show() const override;

    virtual bool unwind(ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment*& env) override;

    bool hasNonOpVariable(Environment* env) const;

    std::string name;
};

