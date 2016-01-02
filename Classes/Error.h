#pragma once
#include "Expression.h"
#include <string>

class Error : public Expression
{
public:
    Error() {}

    virtual std::string show() const override;
};

