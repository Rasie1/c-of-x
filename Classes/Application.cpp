#include "Application.h"
#include "Environment.h"
#include "Lambda.h"
#include "Operation.h"
#include "Error.h"
#include "Function.h"
#include "Identifier.h"
#include "Union.h"

Application::Application()
    : Operator(false, 10)
{
}

ExpPtr Application::operate(ExpPtrArg first,
                            ExpPtrArg second,
                            Environment*& env) const
{
    ExpPtr ret, left, right;

    left = d_cast<Identifier>(first);
    if (left)
        left = env->get(first);
    if (left == nullptr)
        left = first;

    left = left->eval(env);
    right = second;

    auto calculate = [&env](ExpPtrArg l, ExpPtrArg r) -> ExpPtr
    {
        auto function = d_cast<Function>(l);

        if (!function)
            return make_ptr<ErrorWithMessage>("Not a function");

        return function->apply(r, env);
    };

    auto operationLeft  = d_cast<Operation>(left);
    auto operationRight = d_cast<Operation>(right);
    if (operationLeft && d_cast<Union>(operationLeft->op))
        if (operationRight && d_cast<Union>(operationRight->op))
            ret = make_ptr<Operation>(
                        make_ptr<Union>(),
                        make_ptr<Operation>(make_ptr<Union>(),
                                            calculate(operationLeft->left,
                                                      operationRight->left),
                                            calculate(operationLeft->left,
                                                      operationRight->right)),
                        make_ptr<Operation>(make_ptr<Union>(),
                                            calculate(operationLeft->right,
                                                      operationRight->left),
                                            calculate(operationLeft->right,
                                                      operationRight->right)));
        else
            ret = make_ptr<Operation>(make_ptr<Union>(),
                                      calculate(operationLeft->left, right),
                                      calculate(operationLeft->right, right));

    else if (operationRight && d_cast<Union>(operationRight->op))
        ret = make_ptr<Operation>(make_ptr<Union>(),
                                  calculate(left, operationRight->left),
                                  calculate(left, operationRight->right));
    else
        ret = calculate(left, right);

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

