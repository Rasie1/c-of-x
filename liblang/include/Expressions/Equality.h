#pragma once
#include "Expressions/Predicate.h"
#include "Expressions/Operator.h"

class Equality : public Operator
{
public:
    Equality();

    Object operate(const Object&  first,
                   const Object&  second,
                   Environment& env) override;
    Object partialApplyLeft(const Object& e, Environment& env) override;
    Object partialApplyRight(const Object& e, Environment& env) override;

    std::string show() const override;

    static const std::string defaultName;
};


class Equals : public Predicate
{
public:
    Equals(const Object& e) : value(e) {}
    optional<bool> holds(const Object& e, Environment& env) override;

    Object value;

    Object intersect(const Object& other, Environment& env) override;
    Object element(const Environment& env) override;
    bool operator==(const Expression& other) const;

    std::string show() const;
    static const std::string defaultName;
};
