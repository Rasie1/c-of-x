#pragma once
#include "Expressions/Data.h"
#include <string>

class Identifier : public Data
{
public:
    Identifier(const std::string& name)
        : name(name) {}

    bool operator==(const Expression& other) const override;
    std::string show() const override;

    bool hasFreeVariables(const Environment& env) override;
    Object apply(const Object& other, Environment& env) override;

    static Object unwrapIfId(const Object& e, const Environment& env);
    static Object unwrapIfId(const std::shared_ptr<const Expression>& e, const Environment& env);
    bool unapplyVariables(const Object& e, Environment& env) override;

    std::string name;
};

