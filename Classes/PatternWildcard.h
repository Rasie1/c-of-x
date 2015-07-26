#pragma once
#include "Pattern.h"

namespace Language
{

class PatternWildcard : public Pattern
{
public:
    bool match(ExpPtrArg other) const override;
    std::string toString() const override;
};

}
