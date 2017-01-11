#pragma once
#include "Expressions/Predicate.h"
#include "Expressions/Operator.h"
#include "Expressions/Iterable.h"

class MoreThan :
        public Predicate,
        public RangeBegin
{
public:
    MoreThan(ExpPtrArg e) : value(e) {}
    bool holds(ExpPtrArg e, const Environment& env) const override;

    ExpPtr value;

    ExpPtr intersect(ExpPtrArg other, const Environment& env) override;
    ExpPtr unionize(ExpPtrArg other, const Environment& env) override;
    ExpPtr complement(const Environment& env) override;
    bool operator==(const Expression& other) const;

    std::string show() const;
    static const std::string defaultName;

    ExpPtr begin() override;
};

class More : public Operator
{
public:
    More();

    ExpPtr operate(ExpPtrArg  first,
                   ExpPtrArg  second,
                   Environment& env) const override;
    ExpPtr partialApplyLeft(ExpPtrArg e, Environment& env) const override;
    ExpPtr partialApplyRight(ExpPtrArg e, Environment& env) const override;

    std::string show() const override;

    static const std::string defaultName;
};

