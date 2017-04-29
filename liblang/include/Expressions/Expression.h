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

    virtual Object eval(Environment& env) = 0;
    virtual Object apply(const Object& other, Environment& env);
    virtual Object intersect(const Object& other, Environment& env);
    virtual Object unionize(const Object& other, Environment& env);
    virtual Object complement(const Environment& env);
    virtual Object element(const Environment& env);

    virtual bool operator==(const Expression& other) const;
    virtual bool operator!=(const Expression& other) const { return !(*this == other); }

    // used for hash() comparison
    bool         operator< (const Expression& other) const;

    virtual std::string show() const;
    virtual size_t      hash() const;

    virtual bool hasFreeVariables(const Environment& env);

    // returns true if no error occured
    virtual bool unapplyVariables(const Object& e, Environment& env);

    // similar to shared_from_this(), but for Object
    Object thisObject();
};
