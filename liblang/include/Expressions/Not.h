#pragma once
#include "Expressions/Morphism.h"

class Not : public Morphism
{
public:
    Not() {}

    Object apply(const Object& e, Environment& env) override;
    virtual std::string show() const;
    static const std::string defaultName;
};

class Complement : public Data
{
public:
    Complement(const Object& e) : e(e) {}

    virtual std::string show() const;
    static const std::string defaultName;
    
    virtual Object intersect(const Object& other, const Environment& env) override;
    Object complement(const Environment& env) override;

    bool unapplyVariables(const Object& e, Environment& env) override;

    Object e;
};
