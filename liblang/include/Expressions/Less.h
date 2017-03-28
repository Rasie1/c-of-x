#pragma once
#include "Expressions/Predicate.h"
#include "Expressions/Operator.h"
#include "Expressions/Iterable.h"

class LessThan :
        public Predicate,
        public RangeEnd
{
public:
    LessThan(const Object& e) : value(e) {}
    optional<bool> holds(const Object& e, Environment& env) override;

    Object value;

    Object intersect(const Object& other, const Environment& env) override;
    Object unionize(const Object& other, const Environment& env) override;
    Object complement(const Environment& env) override;
    bool operator==(const Expression& other) const;

    std::string show() const;
    static const std::string defaultName;

    Object end() override;
};

class LessOrEqualThan :
        public Predicate,
        public RangeEnd
{
public:
    LessOrEqualThan(const Object& e) : value(e) {}
    optional<bool> holds(const Object& e, Environment& env) override;

    Object value;

    Object intersect(const Object& other, const Environment& env) override;
    Object unionize(const Object& other, const Environment& env) override;
    Object complement(const Environment& env) override;
    bool operator==(const Expression& other) const;

    std::string show() const;
    static const std::string defaultName;

    Object end() override;
};

class Less : public Operator
{
public:
    Less();

    Object operate(const Object&  first,
                   const Object&  second,
                   Environment& env) override;
    Object partialApplyLeft(const Object& e, Environment& env) override;
    Object partialApplyRight(const Object& e, Environment& env) override;

    std::string show() const override;

    static const std::string defaultName;
};

class LessOrEqual : public Operator
{
public:
    LessOrEqual();

    Object operate(const Object&  first,
                   const Object&  second,
                   Environment& env) override;
    Object partialApplyLeft(const Object& e, Environment& env) override;
    Object partialApplyRight(const Object& e, Environment& env) override;

    std::string show() const override;

    static const std::string defaultName;
};
