#pragma once
#include "Predicate.h"
#include "Operator.h"

class LessThan : public Predicate
{
public:
    LessThan(ExpPtrArg e) : value(e) {}
    bool holds(ExpPtrArg e, const Environment& env) const override;

    ExpPtr value;

    ExpPtr intersect(ExpPtrArg other, const Environment& env) override;
    bool operator==(const Expression& other) const;

    std::string show() const;
    static const std::string defaultName;
};

class Less : public Operator
{
public:
    Less();

    ExpPtr operate(ExpPtrArg  first,
                   ExpPtrArg  second,
                   Environment& env) const override;
    ExpPtr partialApply(ExpPtrArg e, Environment& env) const override;

    std::string show() const override;

    static const std::string defaultName;
};

