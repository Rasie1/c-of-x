#pragma once
#include "Expressions/Expression.h"

class RangeBegin
{
public:
    virtual ExpPtr begin() = 0;
};

class RangeEnd
{
public:
    virtual ExpPtr end() = 0;
};

class ValueInIterableSet
{
public:
    virtual ExpPtr next() = 0;
};
