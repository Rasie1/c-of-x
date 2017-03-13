#pragma once
#include "Expressions/Morphism.h"

class Include : public Morphism
{
public:
    ExpPtr apply(ExpPtrArg e, Environment& env) override;
    std::string show() const override;
    static const std::string defaultName;
};

