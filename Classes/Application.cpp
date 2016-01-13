#include "Application.h"
#include "Environment.h"
#include "Lambda.h"
#include "Operation.h"
#include "Error.h"
#include "Function.h"
#include "Variable.h"

Application::Application()
    : Operator(false, 10)
{
}

ExpPtr Application::operate(ExpPtrArg first,
                            ExpPtrArg second,
                            Environment*& env) const
{
    ExpPtr left = d_cast<Identifier>(first);
    if (left)
        left = env->get(first);
    if (left == nullptr)
        left = first;

    auto evaluated = left->eval(env);
    auto function = d_cast<Function>(evaluated);

    if (!function)
        return make_ptr<ErrorWithMessage>("Not a function");

    auto argument = second;//->eval(env);
    auto ret = function->apply(argument, env);

    return ret;
}

std::string Application::show() const
{
    return "";
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

