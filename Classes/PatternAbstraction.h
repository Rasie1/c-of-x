#pragma once
#include "Pattern.h"

namespace Language
{

class PatternAbstraction : public Pattern
{
public:
    PatternAbstraction(const std::string& name)
        : name(name) {}

    bool match(Pattern* other, Environment* env) override;
    std::string toString() const override;

    std::string name;
};

}
