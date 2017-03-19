#pragma once
#include "Expressions/DataType.h"
#include "Expressions/Iterable.h"
class Integer :
        public PlainData,
        public ValueInIterableSet
{
public:
    Integer(long long value = 0);

    bool operator==(const Expression& other) const override;
    std::string show() const override;

    Object next() override;

    long long value;
};
