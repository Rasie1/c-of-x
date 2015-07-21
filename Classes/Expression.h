#pragma once
#include <string>
#include <memory>
#include "DebugHelpers.h"

namespace Language
{

class Environment;

class Pattern;
class Expression : public std::enable_shared_from_this<Expression>
{
public:
    typedef std::shared_ptr<Expression> ExpPtr;
    typedef std::shared_ptr<Pattern>    PatPtr;

    Expression();
    virtual ~Expression();

    ExpPtr eval(Environment*& env) const;
    virtual ExpPtr evaluation(Environment*& env) const;

    virtual ExpPtr apply(const ExpPtr& e,
                         Environment*& env) const;
    virtual PatPtr pattern() const;
    virtual bool match(const ExpPtr& other, Environment* env) const;
    virtual bool unwind(ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment*& env);
    virtual bool isOperator(Environment* env) const;
    virtual std::string toString() const;
    virtual bool hasNonOpVariable(Environment* env) const;
protected:
};

typedef Expression::ExpPtr ExpPtr;
typedef Expression::PatPtr PatPtr;

}
