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

    Expression* evalConstEnv(Environment* env) override;
    std::shared_ptr<Pattern> pattern() const override;

    std::string toString() const override;

    std::string name;
};

}
