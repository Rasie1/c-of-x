#pragma once
#include "DataType.h"

class Function : public DataType
{
public:
    virtual ExpPtr apply(ExpPtrArg e, Environment*& env) const = 0;
};