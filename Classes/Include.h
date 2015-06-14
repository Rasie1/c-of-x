#pragma once
#include "Expression.h"

namespace Language
{

class Include : public Expression
{
public:
    Expression* applyConstEnv(Expression* e, Environment* env) override;
    std::string toString() const override;
    static const std::string defaultName;
};

}
