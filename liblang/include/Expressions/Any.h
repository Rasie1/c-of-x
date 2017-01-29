#pragma once
#include "Expressions/DataType.h"

class Any : public Data
{
public:
    bool unapplyVariables(ExpPtrArg e, Environment& env) const override;
    ExpPtr intersect(ExpPtrArg other, const Environment& env) override;
    virtual std::string show() const;
    static const std::string defaultName;
};
