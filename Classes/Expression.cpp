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

ExpPtr Expression::evalConstEnv(Environment* env) const
{
    return eval(env);
}

ExpPtr Expression::eval(Environment*& env) const
{
    return std::const_pointer_cast<Expression>(shared_from_this());
}

ExpPtr Expression::applyConstEnv(const ExpPtr& e,
                                 Environment* env) const
{
    return apply(e, env);
}

ExpPtr Expression::apply(const ExpPtr& e,
                         Environment*& env) const
{
    return std::make_shared<Operation>(std::make_shared<Application>(),
                                       std::const_pointer_cast<Expression>(shared_from_this()),
                                       e);
}

PatPtr Expression::pattern() const
{
    throw  std::exception();
    return std::make_shared<Pattern>();
}

bool Expression::isOperator(Environment* env) const
{
    return false;
}

std::string Expression::toString() const
{
    return "expr";
}

}
