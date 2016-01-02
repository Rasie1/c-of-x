#include "Application.h"
#include "Expression.h"
#include "PatternOperator.h"
#include "PatternWildcard.h"
#include "Environment.h"
#include "Lambda.h"
#include "Operation.h"

namespace Language
{

Application::Application()
    : Operator(false, 10)
{
}

ExpPtr Application::operate(ExpPtrArg first,
                            ExpPtrArg second,
                            Environment*& env) const
{
    auto function = first->eval(env);
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
    rvalue = make_ptr<Operation>(
                make_ptr<ClosureOperator>(),
                right,
                rvalue);
    return true;
}

}
