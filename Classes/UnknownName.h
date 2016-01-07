#pragma once
#include "Error.h"
#include <string>

class UnknownName : public Expression
{
public:
    UnknownName(const std::string& name)
        : name(name) {}

    std::string show() const override;

    std::string name;
};
