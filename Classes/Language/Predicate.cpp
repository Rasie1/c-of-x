#include "Predicate.h"
#include "Identifier.h"
#include "Environment.h"
#include "Operation.h"
#include "Intersection.h"
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
