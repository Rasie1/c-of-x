#pragma once
#include "Expressions/Expression.h"
#include "Expressions/Morphism.h"
#include "System/Environment.h"

class Environment;
class Closure : public Isomorphism
{
public:
    Closure(ExpPtrArg argument,
            ExpPtrArg body,
            const Environment& env);
    ~Closure();

    ExpPtr apply(ExpPtrArg e, Environment& env) const override;
    // ExpPtr intersect(ExpPtrArg other, const Environment& env) override;
    std::string show() const override;
    bool operator==(const Expression& other) const override;
    bool unapplyVariables(ExpPtrArg e, Environment& env) const override;

    ExpPtr codomain() override;
    ExpPtr domain() override;

    ExpPtr reverse() const override;

    ExpPtr body;
    ExpPtr argument;

    Environment env;
    int envSize;
};

