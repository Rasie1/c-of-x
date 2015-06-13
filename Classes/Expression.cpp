#include "Expression.h"
#include <stdexcept>
#include <iostream>
#include "TypeError.h"
#include "Pattern.h"
#include "Application.h"

#ifdef DEBUG_EVAL
int _debugApps = 0;
#endif

namespace Language
{

Expression::Expression()
{

}

Expression::~Expression()
{

}

Expression* Expression::eval(Environment* env)
{
    return this;
}

Expression* Expression::evalModifyEnv(Environment*& env)
{
    return eval(env);
}

Expression* Expression::apply(Expression* e, Environment* env)
{
    return new Application(this, e);
}

Pattern* Expression::pattern()
{
    throw std::exception();
    return new Pattern();
}

std::string Expression::toString() const
{
    return "expr";
}

Expression* EvalForce::apply(Expression* e, Environment* env)
{
    return e->eval(env)->eval(env);
}

std::string EvalForce::toString() const
{
    return defaultName;
}

const std::string EvalForce::defaultName = "#";


Expression* EvalDelay::apply(Expression* e, Environment* env)
{
    return e;
}

std::string EvalDelay::toString() const
{
    return defaultName;
}

const std::string EvalDelay::defaultName = "\'";


Expression* Print::apply(Expression* e, Environment* env)
{
    std::cout << e->eval(env)->toString() << std::endl;
    return e;
}

std::string Print::toString() const
{
    return defaultName;
}

const std::string Print::defaultName = "print";

}
