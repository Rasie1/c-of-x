#pragma once
#include "Pattern.h"

class PatternWildcard : public Pattern
{
public:
    bool match(ExpPtrArg other) const override;
    std::string show() const override;
};
