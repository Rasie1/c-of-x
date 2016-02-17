#include "Predicate.h"
#include "Identifier.h"
#include "Environment.h"
#include "Operation.h"
#include "Intersection.h"
#include "Lambda.h"
#include "Closure.h"
#include "Void.h"
#include "Any.h"

ExpPtr Predicate::apply(ExpPtrArg e, Environment*& env) const
{
    auto id = d_cast<Identifier>(e);
    if (id)
    {
        ExpPtr x = std::const_pointer_cast<Expression>(shared_from_this());
        env = env->add(id, x);

        auto expr = env->getEqual(id)->eval(env);

        if (d_cast<Any>(expr) || holds(expr, env))
            return e;

        return env->intersect(std::const_pointer_cast<Expression>(shared_from_this()), id);
    }

    auto expr = e->eval(env);
    return holds(expr, env) ? expr : make_ptr<Void>();
}


bool Equals::holds(ExpPtrArg e, Environment* env) const
{
    return *value == *e;
}

std::string Equals::show() const
{
    return defaultName + "(" + value->show() + ")";
}

const std::string Equals::defaultName = "eqv";


ExpPtr Equals::intersect(ExpPtrArg other, Environment* env)
{
    if (std::shared_ptr<Equals> p = d_cast<Equals>(other))
    {
        if (*value == *p->value)
            return value;
        else
            return make_ptr<Void>();
    }
    return make_ptr<Operation>(make_ptr<Intersection>(), value, other);
}


ExpPtr Equality::apply(ExpPtrArg e, Environment*& env) const
{
    return make_ptr<Equals>(e);
}


std::string Equality::show() const
{
    return defaultName;
}

const std::string Equality::defaultName = "eq";
