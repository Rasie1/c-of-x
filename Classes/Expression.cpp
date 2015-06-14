#include "Expression.h"
#include <stdexcept>
#include "TypeError.h"
#include "Pattern.h"
#include "Application.h"

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
    return evalModifyEnv(env);
}

Expression* Expression::evalModifyEnv(Environment*& env)
{
    return this;
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

}
