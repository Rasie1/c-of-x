#pragma once
#include "BaseType.h"

class Integer : public BaseType
{
public:
    Integer(long long value = 0);

    bool match(ExpPtrArg other) const;

    std::string show() const override;

    long long value;
};
