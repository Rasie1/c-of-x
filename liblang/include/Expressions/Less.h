#pragma once
#include "Expressions/Predicate.h"
#include "Expressions/Operator.h"
#include "Expressions/Iterable.h"

class LessThan :
        public Predicate,
        public RangeEnd
{
public:
    LessThan(ExpPtrArg e) : value(e) {}
    optional<bool> holds(ExpPtrArg e, const Environment& env) const override;

    ExpPtr value;

    ExpPtr intersect(ExpPtrArg other, const Environment& env) override;
    ExpPtr unionize(ExpPtrArg other, const Environment& env) override;
    ExpPtr complement(const Environment& env) override;
    bool operator==(const Expression& other) const;

    std::string show() const;
    static const std::string defaultName;

    ExpPtr end() override;
};

class LessOrEqualThan :
        public Predicate,
        public RangeEnd
{
public:
    LessOrEqualThan(ExpPtrArg e) : value(e) {}
    optional<bool> holds(ExpPtrArg e, const Environment& env) const override;

    ExpPtr value;

    ExpPtr intersect(ExpPtrArg other, const Environment& env) override;
    ExpPtr unionize(ExpPtrArg other, const Environment& env) override;
    ExpPtr complement(const Environment& env) override;
    bool operator==(const Expression& other) const;

    std::string show() const;
    static const std::string defaultName;

    ExpPtr end() override;
};

class Less : public Operator
{
public:
    Less();

    ExpPtr operate(ExpPtrArg  first,
                   ExpPtrArg  second,
                   Environment& env) const override;
    ExpPtr partialApplyLeft(ExpPtrArg e, Environment& env) const override;
    ExpPtr partialApplyRight(ExpPtrArg e, Environment& env) const override;

    std::string show() const override;

    static const std::string defaultName;
};

class LessOrEqual : public Operator
{
public:
    LessOrEqual();

    ExpPtr operate(ExpPtrArg  first,
                   ExpPtrArg  second,
                   Environment& env) const override;
    ExpPtr partialApplyLeft(ExpPtrArg e, Environment& env) const override;
    ExpPtr partialApplyRight(ExpPtrArg e, Environment& env) const override;

    std::string show() const override;

    static const std::string defaultName;
};
