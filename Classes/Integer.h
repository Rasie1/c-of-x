#pragma once
#include "DataType.h"

class Integer : public DataType
{
public:
    Integer(long long value = 0);

    bool match(ExpPtrArg other) const;

    std::string show() const override;

    long long value;
};
