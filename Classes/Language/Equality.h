#pragma once
#include "Predicate.h"
#include "Operator.h"

class Equality : public Operator
{
public:
    Equality();

    ExpPtr operate(ExpPtrArg  first,
                   ExpPtrArg  second,
                   Environment*& env) const override;

    std::string show() const override;

    static const std::string defaultName;
};


class Equals : public Predicate
{
public:
    Equals(ExpPtrArg e) : value(e) {}
    bool holds(ExpPtrArg e, Environment* env) const override;

    ExpPtr value;

    ExpPtr intersect(ExpPtrArg other, Environment* env) override;
    bool operator==(const Expression& other) const;

    std::string show() const;
    static const std::string defaultName;
};
