#pragma once
#include "Expressions/Predicate.h"
#include "Expressions/Operator.h"
#include "Expressions/Iterable.h"

class MoreThan :
        public Predicate,
        public RangeBegin
{
public:
    MoreThan(const Object& e) : value(e) {}
    optional<bool> holds(const Object& e, Environment& env) override;

    Object value;

    Object intersect(const Object& other, Environment& env) override;
    Object unionize(const Object& other, Environment& env) override;
    Object complement(const Environment& env) override;
    bool operator==(const Expression& other) const;

    std::string show() const;
    static const std::string defaultName;

    Object begin() override;
};

class More : public Operator
{
public:
    More();

    Object operate(const Object&  first,
                   const Object&  second,
                   Environment& env) override;
    Object partialApplyLeft(const Object& e, Environment& env) override;
    Object partialApplyRight(const Object& e, Environment& env) override;

    std::string show() const override;

    static const std::string defaultName;
};

class MoreOrEqualThan :
        public Predicate,
        public RangeBegin
{
public:
    MoreOrEqualThan(const Object& e) : value(e) {}
    optional<bool> holds(const Object& e, Environment& env) override;

    Object value;

    Object intersect(const Object& other, Environment& env) override;
    Object unionize(const Object& other, Environment& env) override;
    Object complement(const Environment& env) override;
    bool operator==(const Expression& other) const;

    std::string show() const;
    static const std::string defaultName;

    Object begin() override;
};

class MoreOrEqual : public Operator
{
public:
    MoreOrEqual();

    Object operate(const Object&  first,
                   const Object&  second,
                   Environment& env) override;
    Object partialApplyLeft(const Object& e, Environment& env) override;
    Object partialApplyRight(const Object& e, Environment& env) override;

    std::string show() const override;

    static const std::string defaultName;
};

