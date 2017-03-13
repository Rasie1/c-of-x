#pragma once
#include "Expressions/Morphism.h"

class Any : public Morphism
{
public:

    ExpPtr apply(ExpPtrArg e, Environment& env) override;
    bool unapplyVariables(ExpPtrArg e, Environment& env) override;
    ExpPtr intersect(ExpPtrArg other, const Environment& env) override;
    virtual std::string show() const;
    static const std::string defaultName;
};
