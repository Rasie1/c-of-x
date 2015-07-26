#pragma once
#include "Pattern.h"

namespace Language
{

class PatternPredicate : public Pattern
{
    PatternPredicate(Expression* predicate);

    bool match(ExpPtrArg other) const override;
    std::string toString() const override;

    Expression* predicate;
};

}
