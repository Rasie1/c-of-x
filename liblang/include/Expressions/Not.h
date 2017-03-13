#pragma once
#include "Expressions/Morphism.h"

class Not : public Morphism
{
public:
    Not() {}

    ExpPtr apply(ExpPtrArg e, Environment& env) override;
    virtual std::string show() const;
    static const std::string defaultName;
};

class Complement : public Data
{
public:
    Complement(ExpPtrArg e) : e(e) {}

    virtual std::string show() const;
    static const std::string defaultName;
    
    virtual ExpPtr intersect(ExpPtrArg other, const Environment& env) override;
    ExpPtr complement(const Environment& env) override;

    bool unapplyVariables(ExpPtrArg e, Environment& env) override;

    ExpPtr e;
};
