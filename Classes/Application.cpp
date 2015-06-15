#include "Application.h"
#include <string>
#include "Integer.h"
#include "Environment.h"
#include "Variable.h"
#include "Assignment.h"
#include "PatternApplication.h"

namespace Language
{

Application::Application(Expression* function, Expression* argument)
    : function(function),
      argument(argument)
{
}

Application::~Application()
{
    delete function;
    delete argument;
}

Expression* Application::eval(Environment*& env)
{
    DEBUG_PRINT_FUN(function);
    Expression* function = this->function->eval(env);
    DEBUG_PRINT_EVT(function);

    DEBUG_PRINT_ARG(argument);
    auto ret = function->apply(argument, env);
    DEBUG_PRINT_RES(ret);

    return ret;
}

Pattern* Application::pattern()
{
    return new PatternApplication(function->pattern(), argument->pattern());
}

std::string Application::toString() const
{
    return "(" +
            function->toString() +
            " " +
            argument->toString() +
            ")";
}

}
