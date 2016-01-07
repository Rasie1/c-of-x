#pragma once
#include <string>
#include <memory>
#include <vector>
#include "DebugHelpers.h"

class Environment;
class Pattern;
class Variable;
class Expression : public std::enable_shared_from_this<Expression>
{
public:
    typedef std::shared_ptr<Expression> ExpPtr;
    typedef std::shared_ptr<Pattern>    PatPtr;
    typedef std::shared_ptr<Variable>   VarPtr;
    typedef const ExpPtr& ExpPtrArg;
    typedef const PatPtr& PatPtrArg;
    typedef const VarPtr& VarPtrArg;

    Expression();
    virtual ~Expression();

    ExpPtr eval(Environment*& env) const;
    virtual ExpPtr evaluation(Environment*& env) const;


    virtual bool unwind(ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment*& env);

    virtual PatPtr pattern() const;
    virtual bool match(ExpPtrArg other) const;
    virtual bool isOperator(Environment* env) const;
    virtual std::string show() const;
    virtual bool hasNonOpVariable(Environment* env) const;
};

typedef Expression::ExpPtr ExpPtr;
typedef Expression::PatPtr PatPtr;
typedef Expression::VarPtr VarPtr;
typedef Expression::ExpPtr ExpPtrArg;
typedef Expression::PatPtr PatPtrArg;
typedef Expression::VarPtr VarPtrArg;
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


//class Set : public Expression
//{

//};
