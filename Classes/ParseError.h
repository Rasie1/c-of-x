#pragma once
#include "Expression.h"

class ParseError : public Expression
{
public:
    ParseError();
};
