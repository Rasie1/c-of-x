#pragma once
#include "Expression.h"

namespace Language
{

class Pattern : public Expression
{
public:
    Pattern();

    virtual bool match(Pattern* other, Environment* env);
    std::string toString() const override;
};

}
