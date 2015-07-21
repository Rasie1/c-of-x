#pragma once
#include "Expression.h"

namespace Language
{

class Pattern : public Expression
{
public:
    Pattern();

    virtual bool match(const ExpPtr& other,
                       Environment* env) const;
    virtual std::string toString() const override;
    PatPtr pattern() const override;
};

class ReturnPattern : public Expression
{
public:
    ExpPtr apply(const ExpPtr& e,
                 Environment*& env) const override;
    std::string toString() const override;
    static const std::string defaultName;
};

}
