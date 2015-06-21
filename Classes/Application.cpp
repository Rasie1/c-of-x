#include "Application.h"
#include <string>
#include "Expression.h"

namespace Language
{

Application::Application()
    : Operator()
{
}

Expression* Application::operate(Expression* first,
                                 Expression* second,
                                 Environment*& env)
{
    Expression* function = first->eval(env);
    DEBUG_PRINT_EV(function);
    auto ret = function->apply(second, env);

    return ret;
}

std::string Application::toString() const
{
    return "";
}

}
