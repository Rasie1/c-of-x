#pragma once
#include "DataType.h"

class Any : public Data
{
public:
    ExpPtr intersect(ExpPtrArg other, const Environment& env) override;
    virtual std::string show() const;
    static const std::string defaultName;
};
