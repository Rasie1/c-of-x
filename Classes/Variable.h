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

    Expression* evalModifyEnv(Environment*& env);
    Pattern* pattern() override;
    std::string toString() const;

    std::string name;
};

}
