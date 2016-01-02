#pragma once
#include "Expression.h"

namespace Language
{

class Void : public Expression
{
public:
    virtual std::string show() const;
    static const std::string defaultName;
};

}
