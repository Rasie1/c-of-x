#pragma once
#include "Pattern.h"

namespace Language
{

class PatternPredicate : public Pattern
{
    PatternPredicate(Expression* predicate);

    bool match(Pattern* other, Environment* env) override;
    std::string toString() const override;

    Expression* predicate;
};

}
