#pragma once
#include "Expression.h"

namespace Language
{

class Pattern : public Expression
{
public:
    Pattern();

    virtual bool match(const std::shared_ptr<Pattern>& other,
                       Environment* env) const;
    virtual std::string toString() const override;
};

}
