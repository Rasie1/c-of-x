#pragma once
#include "DataType.h"
#include "Iterable.h"
class Integer :
        public PlainData,
        public ValueInIterableSet
{
public:
    Integer(long long value = 0);

    bool operator==(const Expression& other) const override;
    std::string show() const override;

    ExpPtr next() override;

    long long value;
};
