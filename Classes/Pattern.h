#pragma once
#include "Expression.h"

namespace Language
{

class Pattern : public Expression
{
public:
    Pattern();

    virtual bool match(const PatPtr& other,
                       Environment* env) const;
    virtual std::string toString() const override;
};

}
