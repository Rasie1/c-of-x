#pragma once
#include "Expressions/Data.h"
#include <string>

class String : public Data
{
public:
    String(const std::string& value);

    std::string show() const override;

    std::string value;
};
