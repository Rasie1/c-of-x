#include "Expression.h"
#include <stdexcept>
#include "TypeError.h"
#include "Application.h"
#include "Operation.h"
#include "Environment.h"

Expression::Expression()
{

}

Expression::~Expression()
{

}

ExpPtr Expression::eval(Environment*& env) const
{
    auto fromEnv = env->get(std::const_pointer_cast<Expression>(shared_from_this()));
    if (fromEnv)
        return fromEnv;
    return evaluation(env);
}

ExpPtr Expression::evaluation(Environment*& env) const
{
    return std::const_pointer_cast<Expression>(shared_from_this());
}

bool Expression::match(ExpPtrArg other) const
{
    return false;
}

bool Expression::isOperator(Environment* env) const
{
    return false;
}

std::string Expression::show() const
{
    return "expr";
}

bool Expression::hasNonOpVariable(Environment* env) const
{
    return false;
}

bool Expression::unwind(ExpPtr& lvalue,
                        ExpPtr& rvalue,
                        Environment*& env)
{
    return false;
}
