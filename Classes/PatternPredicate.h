#pragma once
#include "Pattern.h"

namespace Language
{

class PatternPredicate : public Pattern
{
    PatternPredicate(Expression* predicate);

    bool match(const std::shared_ptr<Pattern>& other,
               Environment* env) const override;
    std::string toString() const override;

    Expression* predicate;
};

}
