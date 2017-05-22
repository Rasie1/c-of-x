#pragma once
#include "Expressions/Type.h"

class IntegerType : public Type
{
public:
    optional<bool> holds(const Object& e, Environment& env) override;

    std::string show() const;
    static const std::string defaultName;
};

class CastToInt : public Morphism
{
public:
    CastToInt() {}

    Object apply(const Object& e,
                 Environment& env) override;
    virtual std::string show() const;
    static const std::string defaultName;
};
