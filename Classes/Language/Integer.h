#pragma once
#include "DataType.h"

class Integer : public DataType
{
public:
    Integer(long long value = 0);

    bool operator==(const Expression& other) const override;
    std::string show() const override;

    long long value;
};
