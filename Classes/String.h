#pragma once
#include "BaseType.h"
#include <string>

class AdditionOfValue;

class String : public BaseType
{
public:
    String(const std::string& value);

    PatPtr pattern() const override;
    std::string show() const override;


    std::string value;
};
