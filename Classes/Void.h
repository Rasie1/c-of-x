#pragma once
#include "Expression.h"

namespace Language
{

class Void : public Expression
{
public:
    virtual std::string toString() const;
    static const std::string defaultName;
};

}
