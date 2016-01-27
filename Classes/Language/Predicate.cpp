#include "Predicate.h"
#include "Identifier.h"
#include "Environment.h"
#include "Operation.h"
#include "Lambda.h"
#include "Closure.h"
#include "Void.h"

//ExpPtr Assignment::operate(ExpPtrArg first,
//                           ExpPtrArg second,
//                           Environment*& env) const
//{
//    ExpPtr lvalue, rvalue;
//    lvalue = first;

//    if (d_cast<Identifier>(second))
//        rvalue = env->get(second);
//    if (rvalue == nullptr)
//        rvalue = second;

//    if (d_cast<Identifier>(lvalue))
//        env = env->add(lvalue, rvalue);
//    else
//        while (lvalue->unwind(lvalue, rvalue, env));

//    // Enable recursion
//    auto operation = d_cast<Operation>(rvalue);
//    if (operation && d_cast<Lambda>(operation->op))
//        env = env->add(lvalue, rvalue);

//    return second;
//}

ExpPtr Predicate::apply(ExpPtrArg e, Environment*& env) const
{
    return nullptr;
//    ExpPtr value;
//    if (d_cast<Identifier>(e))
//        value = env->get(e);
//    if (value == nullptr)
//        value = e;

//    auto holds = holds(e);

//    if (holds)
//        return e;
//    else
//        return make_ptr<Void>();

//    ExpPtr arg;
//    arg = d_cast<Identifier>(e);
//    if (arg)
//        arg = env->get(e);
//    else
//        arg = e;
//    auto evaluated = arg->eval(env);
//    return evaluated;
}
