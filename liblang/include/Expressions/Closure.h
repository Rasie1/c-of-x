#pragma once
#include "Expressions/Expression.h"
#include "Expressions/Morphism.h"
#include "System/Environment.h"

class Environment;
class Closure : public Morphism
{
public:
    Closure(const Object& argument,
            const Object& body,
            const Environment& env);
    ~Closure();

    Object apply(const Object& e, Environment& env) override;
    // Object intersect(const Object& other, Environment& env) override;
    std::string show() const override;
    bool operator==(const Expression& other) const override;
    bool unapplyVariables(const Object& e, Environment& env) override;

    Object codomain() override;
    Object domain() override;

    Object inverse() override;

    Object body;
    Object argument;

    Environment env;
    int envSize;
};

