#pragma once
#include <string>
#include <memory>
#include <typeinfo>
#include <stdexcept>

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
    virtual bool unapplyVariables(ExpPtrArg e, Environment& env) const;
};

typedef Expression::ExpPtr ExpPtr;
typedef Expression::ExpPtr ExpPtrArg;
typedef Expression::CExpPtr CExpPtr;
typedef Expression::CExpPtr CExpPtrArg;

template <class T, class... Args>
constexpr auto make_ptr(Args&&... args)
 -> decltype(std::make_shared<T>(std::forward<Args>(args)...))
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}
template <class T, class... Args>
constexpr auto s_cast(Args&&... args)
 -> decltype(std::static_pointer_cast<T>(std::forward<Args>(args)...))
{
    return std::static_pointer_cast<T>(std::forward<Args>(args)...);
}
template <class T, class... Args>
constexpr auto d_cast(Args&&... args)
 -> decltype(std::dynamic_pointer_cast<T>(std::forward<Args>(args)...))
{
    return std::dynamic_pointer_cast<T>(std::forward<Args>(args)...);
}
template <class T>
constexpr bool checkType(const std::shared_ptr<const Expression>& e)
{
    return typeid(*e) == typeid(T);
}
