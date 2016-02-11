#include "Predicate.h"
#include "Identifier.h"
#include "Environment.h"
#include "Operation.h"
#include "Lambda.h"
#include "Closure.h"
#include "Void.h"

ExpPtr Predicate::apply(ExpPtrArg e, Environment*& env) const
{
    auto t = shared_from_this();
    auto x = env->process(e, s_cast<const Function>(t));

    return x.first;
//    ExpPtr value;
//    if (d_cast<Identifier>(e))
//        value = env->get(e);
//    if (value == nullptr)
//        value = e;

//    if (holds(e, env))
//        return e;
//    else
//        return make_ptr<Void>();
}


bool Equals::holds(ExpPtrArg e, Environment*& env) const
{
    return *value == *e;
}

std::string Equals::show() const
{
    return defaultName + "(" + value->show() + ")";
}

const std::string Equals::defaultName = "eqv";



ExpPtr Equality::apply(ExpPtrArg e, Environment*& env) const
{
    return make_ptr<Equals>(e);
}


std::string Equality::show() const
{
    return defaultName;
}

const std::string Equality::defaultName = "eq";
