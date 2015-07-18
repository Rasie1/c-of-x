#include "Expression.h"
#include <stdexcept>
#include "TypeError.h"
#include "Pattern.h"
#include "Application.h"
#include "Operation.h"
#include "Environment.h"

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
    auto fromEnv = env->get(pattern());
    if (fromEnv)
        return fromEnv;
    return evaluation(env);
}

ExpPtr Expression::evaluation(Environment*& env) const
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

bool Expression::unwind(ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment*& env)
{
    return false;
}

}
