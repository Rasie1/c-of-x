#pragma once
#include "DataType.h"
#include <string>

class String : public DataType
{
public:
    String(const std::string& value);

    std::string show() const override;

    std::string value;
};
