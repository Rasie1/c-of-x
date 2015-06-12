#pragma once
#include "BaseType.h"

namespace Language
{

class AdditionOfValue;

class Integer : public BaseType
{
public:
    Integer(long long value);

    Pattern* pattern() override;
    std::string toString() const override;


    long long value;
};

}
