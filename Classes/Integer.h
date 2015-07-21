#pragma once
#include "BaseType.h"

namespace Language
{

class AdditionOfValue;

class Integer : public BaseType
{
public:
    Integer(long long value = 0);

    PatPtr pattern() const override;
    std::string toString() const override;


    long long value;
};

class IntegerSet
{
public:
    IntegerSet() {}
};

}
