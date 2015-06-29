#pragma once
#include "Pattern.h"

namespace Language
{

class PatternWildcard : public Pattern
{
public:
    bool match(const PatPtr& other,
               Environment* env) const override;
    std::string toString() const override;
};

}
