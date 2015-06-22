#include "Application.h"
#include "Expression.h"

namespace Language
{

Application::Application()
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

}
