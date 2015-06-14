#pragma once
#include "Expression.h"

namespace Language
{

class Print : public Expression
{
public:
    Print() {}

    Expression* applyConstEnv(Expression* e, Environment* env) override;
    virtual std::string toString() const;
    static const std::string defaultName;
};

}
