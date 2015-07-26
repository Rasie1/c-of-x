#pragma once
#include "Pattern.h"

namespace Language
{

class PatternApplication : public Pattern
{
public:
    PatternApplication(PatPtrArg body,
                       PatPtrArg argument);

    bool match(ExpPtrArg other,
               Environment* env) const override;
    std::string toString() const override;

    PatPtr body;
    PatPtr argument;
};

}
