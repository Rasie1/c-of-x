#pragma once
#include "Expressions/TypedData.h"
#include "Expressions/Iterable.h"
#include "Expressions/IntegerType.h"

class Integer :
        public TypedData<IntegerType>,
        public ValueInIterableSet
{
public:
    Integer(long long value = 0);

    Object equals(const Object& other, Environment& env) override;
    bool operator==(const Expression& other) const override;
    std::string show() const override;

    Object next() override;

    long long value;
};
