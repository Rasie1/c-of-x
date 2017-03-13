#pragma once
#include <string>
#include <memory>
#include <typeinfo>
#include <stdexcept>
#include <boost/optional.hpp>

class Environment;
class Expression : public std::enable_shared_from_this<Expression>
{
public:
    typedef std::shared_ptr<Expression> ExpPtr;
    typedef const ExpPtr& ExpPtrArg;
    typedef std::shared_ptr<const Expression> CExpPtr;
    typedef const CExpPtr& CExpPtrArg;

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

typedef Expression::ExpPtr ExpPtr;
typedef Expression::ExpPtr ExpPtrArg;
typedef Expression::CExpPtr CExpPtr;
typedef Expression::CExpPtr CExpPtrArg;

using boost::optional;
using boost::none;
using boost::make_optional;
using std::static_pointer_cast;
using std::dynamic_pointer_cast;

template <class T, class... Args>
constexpr auto make_ptr(Args&&... args)
 -> decltype(std::make_shared<T>(std::forward<Args>(args)...))
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}
