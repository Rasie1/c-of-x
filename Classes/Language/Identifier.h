#pragma once
#include "DataType.h"
#include <string>

class Identifier : public DataType
{
public:
    Identifier(const std::string& name)
        : name(name) {}

    bool operator==(const Expression& other) const override;
    std::string show() const override;

    bool hasFreeVariables(const Environment& env) const;

    static ExpPtr unwrapIfId(ExpPtrArg e, const Environment& env);
    bool unapplyVariables(ExpPtrArg e, Environment& env) const;

    std::string name;
};

