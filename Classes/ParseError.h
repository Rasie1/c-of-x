#pragma once
#include "Expression.h"

namespace Language
{

class ParseError : public Expression
{
public:
    ParseError();
};

}
