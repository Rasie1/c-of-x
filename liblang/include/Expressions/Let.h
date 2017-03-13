#pragma once
#include "Expressions/Morphism.h"
#include "System/Environment.h"

class Let : public Morphism
{
public:
    ExpPtr apply(ExpPtrArg e, Environment& env) override;
    std::string show() const override;
    static const std::string defaultName;
};

class LetContext : public Morphism
{
public:
    LetContext(const Environment& env);

    ExpPtr apply(ExpPtrArg e, Environment& env) override;
    std::string show() const override;

    Environment env;
};
