#pragma once
#include "Predicate.h"

class ValueInSet : public DataType
{
public:
    ValueInSet(ExpPtrArg set) : set(set) {}
    virtual std::string show() const;

    ExpPtr set;
};
