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

ExpPtr Expression::eval(Environment*& env) const
{
    return std::const_pointer_cast<Expression>(shared_from_this());
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
