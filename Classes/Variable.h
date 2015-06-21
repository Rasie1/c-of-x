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

    Expression* eval(Environment*& env);
    std::shared_ptr<Pattern> pattern() const override;
    bool isOperator(Environment* env) override;
    std::string toString() const;

    std::string name;
};

}
