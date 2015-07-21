#pragma once
#include "Pattern.h"

namespace Language
{

class PatternApplication : public Pattern
{
public:
    PatternApplication(const PatPtr& body,
                       const PatPtr& argument);

    bool match(const ExpPtr& other,
               Environment* env) const override;
    std::string toString() const override;

    PatPtr body;
    PatPtr argument;
};

}
