#pragma once
#include "Expression.h"

namespace Language
{

class Void : public Expression
{
public:
    Void() {}

    virtual std::string toString() const;
};

}
