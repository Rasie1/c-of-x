#include "Application.h"
#include "Expression.h"
#include "PatternOperator.h"
#include "PatternWildcard.h"
#include "Environment.h"
#include "Lambda.h"
#include "Operation.h"
#include "Error.h"

Application::Application()
    : Operator(false, 10)
{
}

ExpPtr Application::operate(ExpPtrArg first,
                            ExpPtrArg second,
                            Environment*& env) const
{
    auto function = d_cast<Function>(first->eval(env));


    if (!function)
        return make_ptr<ErrorWithMessage>("Not a function");

    auto argument = second;//->eval(env);
    DEBUG_PRINT_EV(function);
    auto ret = function->apply(argument, env);

    return ret;
}

std::string Application::show() const
{
    return "";
}

PatPtr Application::pattern() const
{
    return make_ptr<PatternOperator>(
                name,
                s_cast<Operator>(
                    std::const_pointer_cast<Expression>(
                        shared_from_this())));
}

bool Application::unwind(ExpPtr& left,
                         ExpPtr& right,
                         ExpPtr& lvalue,
                         ExpPtr& rvalue,
                         Environment*& env)
{
    lvalue = left;
    rvalue = make_ptr<Operation>(make_ptr<Lambda>(),
                                 right,
                                 rvalue);
    return true;
}

