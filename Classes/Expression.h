#pragma once
#include <string>
#include <memory>
#include <vector>
#include "DebugHelpers.h"

namespace Language
{

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

    virtual ExpPtr apply(ExpPtrArg e,
                         Environment*& env) const;
    virtual PatPtr pattern() const;
    virtual bool match(ExpPtrArg other, Environment* env) const;
    virtual bool unwind(ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment*& env);
    virtual bool isOperator(Environment* env) const;
    virtual std::string toString() const;
    virtual bool hasNonOpVariable(Environment* env) const;
    virtual void getAllVariables(
            std::vector<VarPtr>& variables);
protected:
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


//class Set : public Expression
//{

//};

}
