#pragma once
#include <string>
#include <stdexcept>
#include <boost/optional.hpp>
#include "System/Object.h"

class Environment;
class Expression : public std::enable_shared_from_this<Expression>
{
public:
    Expression();

    virtual ExpPtr eval(Environment& env) const = 0;
    virtual ExpPtr apply(ExpPtrArg other, Environment& env);
    virtual ExpPtr intersect(ExpPtrArg other, const Environment& env);
    virtual ExpPtr unionize(ExpPtrArg other, const Environment& env);
    virtual ExpPtr complement(const Environment& env);
    virtual ExpPtr element(const Environment& env);

    virtual bool operator==(const Expression& other) const;
    virtual bool operator!=(const Expression& other) const { return !(*this == other); }

    // used for hash() comparison
    bool         operator< (const Expression& other) const;

    virtual std::string show() const;
    virtual size_t      hash() const;

    virtual bool hasFreeVariables(const Environment& env) const;

    // returns true if no error occured
    virtual bool unapplyVariables(ExpPtrArg e, Environment& env);
};
