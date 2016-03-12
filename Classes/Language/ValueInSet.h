#pragma once
#include "Predicate.h"

class ValueInSet : public DataType
{
public:
    ValueInSet(ExpPtrArg set) : set(set) {}

    ExpPtr eval(Environment& env) const override;

    virtual std::string show() const;

    ExpPtr set;
};
