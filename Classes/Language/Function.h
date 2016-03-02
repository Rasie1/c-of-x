#pragma once
#include "DataType.h"

class Function : public DataType
{
public:
    virtual ExpPtr apply(ExpPtrArg e, Environment& env) const = 0;
    virtual ExpPtr reverse() const;
    virtual ExpPtr intersect(ExpPtrArg other, const Environment& env) override;
//    void unapplyVariables(ExpPtrArg e, Environment& env) const override;
};
