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

    virtual ExpPtr evalConstEnv(Environment* env) const;
    virtual ExpPtr eval(Environment*& env) const;

    virtual ExpPtr applyConstEnv(const ExpPtr& e,
                                 Environment* env) const;
    virtual ExpPtr apply(const ExpPtr& e,
                         Environment*& env) const;

    virtual PatPtr pattern() const;
    virtual bool isOperator(Environment* env) const;
    virtual std::string toString() const;
};

typedef Expression::ExpPtr ExpPtr;
typedef Expression::PatPtr PatPtr;

}
