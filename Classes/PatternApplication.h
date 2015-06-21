#pragma once
#include "Pattern.h"

namespace Language
{

class PatternApplication : public Pattern
{
public:
    PatternApplication(const std::shared_ptr<Pattern>& body,
                       const std::shared_ptr<Pattern>& argument);

    bool match(const std::shared_ptr<Pattern>& other,
               Environment* env) const override;
    std::string toString() const override;

    std::shared_ptr<Pattern> body;
    std::shared_ptr<Pattern> argument;
};

}
