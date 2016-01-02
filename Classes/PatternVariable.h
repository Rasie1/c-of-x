#pragma once
#include "Pattern.h"

class PatternVariable : public Pattern
{
public:
    PatternVariable(const std::string& name)
        : name(name) {}

    bool match(ExpPtrArg other) const override;
    std::string show() const override;

    std::string name;
};

