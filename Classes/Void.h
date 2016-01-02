#pragma once
#include "Expression.h"

class Void : public Expression
{
public:
    virtual std::string show() const;
    static const std::string defaultName;
};
