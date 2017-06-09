#pragma once
#include "Expressions/Data.h"

class Any : public Data
{
public:

    Object apply(const Object& e, Environment& env) override;
    bool unapplyVariables(const Object& e, Environment& env) override;
    Object intersect(const Object& other, Environment& env) override;
    virtual std::string show() const;
    static const std::string defaultName;
};
