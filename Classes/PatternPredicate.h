#pragma once
#include "Pattern.h"

namespace Language
{

class PatternPredicate : public Pattern
{
    PatternPredicate(Expression* predicate);

    bool match(const PatPtr& other,
               Environment* env) const override;
    std::string toString() const override;

    Expression* predicate;
};

}
