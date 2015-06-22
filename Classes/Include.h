#pragma once
#include "Expression.h"

namespace Language
{

class Include : public Expression
{
public:
    ExpPtr apply(const ExpPtr& e, Environment*& env) const override;
    std::string toString() const override;
    static const std::string defaultName;
};

}
