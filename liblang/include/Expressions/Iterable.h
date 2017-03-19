#pragma once
#include "Expressions/Expression.h"

class RangeBegin
{
public:
    virtual Object begin() = 0;
};

class RangeEnd
{
public:
    virtual Object end() = 0;
};

class ValueInIterableSet
{
public:
    virtual Object next() = 0;
};
