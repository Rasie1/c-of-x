#pragma once
#include <string>
#include <memory>
#include <vector>
#include "DebugHelpers.h"

class Environment;
class Expression : public std::enable_shared_from_this<Expression>
{
public:
    typedef std::shared_ptr<Expression> ExpPtr;
    typedef const ExpPtr& ExpPtrArg;

    Expression();

    virtual ExpPtr eval(Environment*& env) const = 0;

    virtual bool unwind(ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment*& env);

    virtual bool operator==(const Expression& other) const;
    virtual ExpPtr intersect(ExpPtrArg other, Environment* env);

    virtual std::string show() const;
    virtual std::string hash() const;
    virtual bool hasNonOpVariable(Environment* env) const;
};

typedef Expression::ExpPtr ExpPtr;
typedef Expression::ExpPtr ExpPtrArg;
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
