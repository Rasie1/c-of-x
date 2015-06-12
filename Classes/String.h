#pragma once
#include "BaseType.h"
#include <string>

namespace Language
{

class AdditionOfValue;

class String : public BaseType
{
public:
    String(const std::string& value);

    Pattern* pattern() override;
    std::string toString() const override;


    std::string value;
};

}
