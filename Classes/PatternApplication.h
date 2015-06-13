#pragma once
#include "Pattern.h"

namespace Language
{

class PatternApplication : public Pattern
{
public:
    PatternApplication(Pattern* body, Pattern* argument);

    bool match(Pattern* other, Environment* env) override;
    std::string toString() const override;

    Pattern* body;
    Pattern* argument;
};

}
