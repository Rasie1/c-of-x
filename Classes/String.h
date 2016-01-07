#pragma once
#include "BaseType.h"
#include <string>

class String : public BaseType
{
public:
    String(const std::string& value);

    std::string show() const override;

    std::string value;
};
