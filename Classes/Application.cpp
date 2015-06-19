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

    DEBUG_PRINT_FUN(first);
    Expression* function = first->eval(env);
    DEBUG_PRINT_EVT(function);

    DEBUG_PRINT_ARG(second);
    auto ret = function->apply(second, env);
    DEBUG_PRINT_RES(ret);

    return ret;
}

std::string Application::toString() const
{
    return "<]";
}

}
