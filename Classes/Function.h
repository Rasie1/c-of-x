#pragma once
#include "Expression.h"

namespace Language
{

class Function : public Expression
{
public:
    Function(Expression* body,
             const std::shared_ptr<Pattern>& argument);
    ~Function();

    Expression* eval(Environment*& env) override;
    Expression* apply(Expression* e, Environment*& env) override;
    std::string toString() const override;

    Expression* body;
    std::shared_ptr<Pattern> argument;
};

}
