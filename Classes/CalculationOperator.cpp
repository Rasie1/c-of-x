#include "CalculationOperator.h"
#include "Operation.h"
#include "Integer.h"
#include "Subtraction.h"
#include "Identifier.h"
#include "TypeError.h"
#include "Environment.h"

CalculationOperator::CalculationOperator(bool isRightAssociative, int priority)
    : Operator(isRightAssociative, priority)
{
}

ExpPtr CalculationOperator::operate(ExpPtrArg first,
                                    ExpPtrArg second,
                                    Environment*& env) const
{
    ExpPtr l, r;
    l = d_cast<Identifier>(first);
    if (l)
        l = env->get(l);
    else
        l = first;
    r = d_cast<Identifier>(second);
    if (r)
        r = env->get(r);
    else
        r = second;
    l = l->eval(env);
    r = r->eval(env);

    return calculate(l, r);
}

//bool CalculationOperator::unwind(ExpPtr& left,
//                      ExpPtr& right,
//                      ExpPtr& lvalue,
//                      ExpPtr& rvalue,
//                      Environment*& env)
//{
//    auto newEnv = env;
//    if (left->hasNonOpVariable(env))
//    {
//        if (right->hasNonOpVariable(env))
//            return false;
//        lvalue = left;
//        rvalue = make_ptr<Operation>(
//                    make_ptr<Subtraction>(),
//                    rvalue,
//                    right);
//        return true;
//    }
//    else if (right->hasNonOpVariable(env))
//    {
//        lvalue = right;
//        rvalue = make_ptr<Operation>(
//                    make_ptr<Subtraction>(),
//                    rvalue,
//                    left);
//        return true;
//    }
//    return false;
//}
