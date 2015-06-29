#include "Application.h"
#include "Expression.h"
#include "Pattern.h"
#include "PatternOperator.h"
#include "PatternVariable.h"
#include "Environment.h"

namespace Language
{

Application::Application()
    : Operator(false, 10)
{
}

ExpPtr Application::operate(const ExpPtr& first,
                            const ExpPtr& second,
                            Environment*& env) const
{
    auto function = first->eval(env);
    DEBUG_PRINT_EV(function);
    auto ret = function->apply(second, env);

    return ret;
}

std::string Application::toString() const
{
    return "";
}

PatPtr Application::pattern() const
{
    return std::make_shared<PatternOperator>(
                name,
                std::static_pointer_cast<Operator>(
                    std::const_pointer_cast<Expression>(
                        shared_from_this())));
}

PatPtr Application::leftPattern(const ExpPtr& e) const
{
    return e->pattern();
}

PatPtr Application::rightPattern(const ExpPtr& e) const
{
    return std::make_shared<PatternWildcard>();
}

}
