#pragma once
#include "Expressions/DataType.h"

class Void : public Data
{
public:
    virtual std::string show() const;
    static const std::string defaultName;

    Object intersect(const Object& other, Environment& env) override;

    bool unapplyVariables(const Object& e, Environment& env) override;
};
