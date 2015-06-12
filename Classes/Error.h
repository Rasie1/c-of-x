#pragma once
#include "Expression.h"
#include <string>

namespace Language
{

class Error : public Expression
{
public:
    Error() {}

    virtual std::string toString() const override;
};

}
