#pragma once
#include "BaseType.h"

class AdditionOfValue;

class Integer : public BaseType
{
public:
    Integer(long long value = 0);

    PatPtr pattern() const override;
    bool match(ExpPtrArg other) const;

    std::string show() const override;

    long long value;
};

class IntegerSet
{
public:
    IntegerSet() {}
};
