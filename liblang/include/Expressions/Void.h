#pragma once
#include "DataType.h"

class Void : public Data
{
public:
    virtual std::string show() const;
    static const std::string defaultName;

    ExpPtr intersect(ExpPtrArg other, const Environment& env) override;

    bool unapplyVariables(ExpPtrArg e, Environment& env) const override;
};
