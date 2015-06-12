#pragma once
#include "Error.h"
#include <string>

namespace Language
{

class UnknownName : public Expression
{
public:
    UnknownName(const std::string& name)
        : name(name) {}

    Expression* eval(Environment* env) override;
    Pattern* pattern() override;

    std::string toString() const override;

    std::string name;
};

}
