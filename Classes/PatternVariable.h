#pragma once
#include "Pattern.h"

namespace Language
{

class PatternVariable : public Pattern
{
public:
    PatternVariable(const std::string& name)
        : name(name) {}

    bool match(ExpPtrArg other,
               Environment* env) const override;
    std::string toString() const override;

    std::string name;
};

}
