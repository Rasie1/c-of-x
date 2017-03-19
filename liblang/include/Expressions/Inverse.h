#pragma once
#include "Expressions/Morphism.h"

class Inverse : public Morphism
{
public:
    Object apply(const Object& e, Environment& env) override;
    std::string show() const override;
    static const std::string defaultName;
};

