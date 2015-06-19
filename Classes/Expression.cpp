#include "Expression.h"
#include <stdexcept>
#include "TypeError.h"
#include "Pattern.h"
#include "Application.h"
#include "Operation.h"

namespace Language
{

Expression::Expression()
{

}

Expression::~Expression()
{

}

Expression* Expression::evalConstEnv(Environment* env)
{
    return eval(env);
}

Expression* Expression::eval(Environment*& env)
{
    return this;
}

Expression* Expression::applyConstEnv(Expression* e, Environment* env)
{
    return apply(e, env);
}

Expression* Expression::apply(Expression* e, Environment*& env)
{
    return new Operation(new Application(), this, e);
}

Pattern* Expression::pattern()
{
    throw std::exception();
    return new Pattern();
}

bool Expression::isOperator(Environment* env)
{
    return false;
}

std::string Expression::toString() const
{
    return "expr";
}

}
