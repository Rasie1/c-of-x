#pragma once
#include "Pattern.h"

class PatternPredicate : public Pattern
{
    PatternPredicate(Expression* predicate);

    bool match(ExpPtrArg other) const override;
    std::string show() const override;

    Expression* predicate;
};
