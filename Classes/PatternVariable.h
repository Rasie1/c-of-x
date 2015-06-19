#pragma once
#include "Pattern.h"

namespace Language
{

class PatternVariable : public Pattern
{
public:
    PatternVariable(const std::string& name)
        : name(name) {}

    bool match(Pattern* other, Environment* env) override;
    std::string toString() const override;

    std::string name;
};

}
