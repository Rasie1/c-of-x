#pragma once
#include "Pattern.h"

class PatternApplication : public Pattern
{
public:
    PatternApplication(PatPtrArg body,
                       PatPtrArg argument);

    bool match(ExpPtrArg other) const override;
    std::string show() const override;

    PatPtr body;
    PatPtr argument;
};
