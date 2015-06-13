#pragma once
#include "Expression.h"

namespace Language
{

class Print : public Expression
{
public:
    Print() {}

    Expression* apply(Expression* e, Environment* env) override;
    virtual std::string toString() const;
    static const std::string defaultName;
};

}
